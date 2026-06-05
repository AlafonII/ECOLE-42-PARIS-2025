#!/bin/bash

# ---------------------------------------------------------------
#  WordPress Initialization Script
#  This script installs and configures WordPress automatically
#  using environment variables and WP-CLI commands.
# ---------------------------------------------------------------

# ---------------------------------------------------------------
#  Wait for MariaDB to be ready before continuing
# ---------------------------------------------------------------
echo "Inception : Waiting for MariaDB to be ready..."
until mariadb -h$DB_HOST -u$DB_USER -p$DB_USER_PASSWORD -e "SELECT 1;" > /dev/null 2>&1; do
    echo "MariaDB not ready yet, retrying in 2s..."
    sleep 2
done
echo "MariaDB is ready, continuing WordPress setup."


# ---------------------------------------------------------------
#  Step 1: Check if WordPress directory exists
# ---------------------------------------------------------------
# - If not present, download the WordPress core files
if ! [ -d $WP_PATH ]; then
    echo "Inception : Download core WordPress"
    wp core download --path=$WP_PATH --allow-root
fi

# Change to the WordPress installation directory
cd $WP_PATH;

# ---------------------------------------------------------------
#  Step 2: Check for existing wp-config.php and database settings
# ---------------------------------------------------------------
# - If wp-config.php exists and DB_PASSWORD is set, skip configuration
if [ -f wp-config.php ] && wp core is-installed --allow-root; then
    echo "Inception : WordPress already installed"
else
    # ---------------------------------------------------------------
    #  Step 3: Create a new wp-config.php file
    # ---------------------------------------------------------------
    cp wp-config-sample.php wp-config.php

    # Configure database connection settings
    wp config set --allow-root DB_HOST $DB_HOST --path="."
    wp config set --allow-root DB_NAME $DB_DATABASE --path="." 
    wp config set --allow-root DB_USER $DB_USER --path="."
    wp config set --allow-root DB_PASSWORD "${DB_USER_PASSWORD}" --path="." --quiet
    wp config set --allow-root table_prefix $DB_TABLE_PREFIX --path="."

    # Disable debugging for production environment
    wp config set --allow-root WP_DEBUG false --path="." --raw
    wp config set --allow-root WP_DEBUG_LOG false --path="." --raw

    # Randomize authentication salts for security
    wp config shuffle-salts --allow-root

    echo "wp-config.php file generated"

    # ---------------------------------------------------------------
    #  Step 4: Install and configure WordPress
    # ---------------------------------------------------------------
    echo "Installing WordPress"
    wp core install --allow-root \
        --path="." \
        --url=$DOMAIN_NAME \
        --title=$WP_TITLE \
        --admin_user=$WP_ADMIN \
        --admin_password=$WP_ADMIN_PASSWORD \
        --admin_email=$WP_ADMIN_EMAIL

    # Update all installed plugins
    wp plugin update --path="." --allow-root --all

    # ---------------------------------------------------------------
    #  Step 5: Create a secondary user (author)
    # ---------------------------------------------------------------
    wp user create --path=$WP_PATH --allow-root \
        $WP_USER $WP_USER_EMAIL --user_pass=$WP_USER_PASSWORD \
        --role=author --porcelain
fi

# ---------------------------------------------------------------
#  Step 6: Start PHP-FPM in foreground mode
# ---------------------------------------------------------------
php-fpm7.4 -F
