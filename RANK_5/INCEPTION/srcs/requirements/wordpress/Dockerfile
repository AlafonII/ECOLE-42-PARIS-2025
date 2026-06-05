# ---------------------------------------------------------------
#  WordPress / PHP-FPM Dockerfile
#  This Dockerfile builds a PHP-FPM container capable of running
#  WordPress with MariaDB connectivity and WP-CLI for automation
# ---------------------------------------------------------------

# ---------------------------------------------------------------
#  Base image: Debian Bullseye
# ---------------------------------------------------------------
FROM debian:bullseye

# ---------------------------------------------------------------
#  Step 1: Install PHP, MariaDB client, and WP-CLI dependencies
# ---------------------------------------------------------------
# RUN apt-get -o Acquire::Check-Valid-Until=false -o Acquire::Check-Date=false update && \
# Update package lists, ignoring outdated timestamp validation
RUN apt-get update && \
# Install essential packages: curl, PHP core, PHP-FPM, MySQL extension, and MariaDB client
    apt-get install -y curl php php-fpm php-mysql mariadb-client && \
# Create the PHP runtime directory required for PHP-FPM to operate
    mkdir -p /run/php && \
# Download the WP-CLI Phar (PHP Archive) file from the official GitHub builds
    curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar && \
# Make the downloaded WP-CLI file executable
    chmod +x wp-cli.phar && \
# Move the executable into /usr/local/bin so it can be called globally as “wp”
    mv wp-cli.phar /usr/local/bin/wp && \
# Update WP-CLI to ensure the latest stable release is installed
    wp cli update

# ---------------------------------------------------------------
#  Step 2: Copy PHP-FPM configuration file
# ---------------------------------------------------------------
# - Use a custom pool configuration for WordPress (www.conf)
COPY ./conf/www.conf /etc/php/7.4/fpm/pool.d/

# ---------------------------------------------------------------
#  Step 3: Copy container entrypoint script
# ---------------------------------------------------------------
# - Add executable startup script that handles WordPress setup
COPY --chmod=0755 ./tools/entrypoint.sh /usr/local/bin/

# ---------------------------------------------------------------
#  Step 4: Expose PHP-FPM port
# ---------------------------------------------------------------
EXPOSE 9000

# ---------------------------------------------------------------
#  Step 5: Set entrypoint to launch PHP-FPM via the setup script
# ---------------------------------------------------------------
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
