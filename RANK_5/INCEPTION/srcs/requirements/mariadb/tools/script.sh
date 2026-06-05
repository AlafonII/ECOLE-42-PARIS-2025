#!/bin/bash

# ---------------------------------------------------------------
#  This script configures and starts the MariaDB server if needed.
# ---------------------------------------------------------------

# Check if MariaDB is already running by looking for its PID file
if [ ! -f "/run/mysqld/mysqld.pid" ]; then

	# ---------------------------------------------------------------
	#  Step 1: Configure MariaDB to listen on all network interfaces
	# ---------------------------------------------------------------
	# Replace "127.0.0.1" (localhost only) with "0.0.0.0" (any IP)
	sed -i 's/= 127.0.0.1/= 0.0.0.0/' /etc/mysql/mariadb.conf.d/50-server.cnf

	# ---------------------------------------------------------------
	#  Step 2: Ensure port 3306 is defined before 'basedir'
	# ---------------------------------------------------------------
	# This line inserts the port configuration line before 'basedir'
	sed -i 's/basedir/port\t\t\t\t\t= 3306\nbasedir/' /etc/mysql/mariadb.conf.d/50-server.cnf

	echo "Inception: MariaDB config (50-server.cnf) updated."

	# ---------------------------------------------------------------
	#  Step 3: Check if the database folder already exists
	# ---------------------------------------------------------------
	if [ ! -d "/var/lib/mysql/${DB_DATABASE}" ]; then
		echo "Inception: Creating '${DB_DATABASE}' database..."

		# Start MariaDB service temporarily to perform setup
		service mariadb start

		# ---------------------------------------------------------------
		#  Step 4: Create database and users with environment variables
		# ---------------------------------------------------------------

		# Create the main database if it doesn’t exist
		mysql -e "CREATE DATABASE IF NOT EXISTS ${DB_DATABASE};"

		# Create the application user and give them full access
		mysql -e "CREATE USER IF NOT EXISTS '${DB_USER}'@'%' IDENTIFIED BY '${DB_USER_PASSWORD}';"
		mysql -e "GRANT ALL PRIVILEGES ON *.* TO '${DB_USER}'@'%' IDENTIFIED BY '${DB_USER_PASSWORD}';"

		# Allow remote root access and set its password
		mysql -e "GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY '${DB_ROOT_PASSWORD}';"

		# Apply privilege changes
		mysql -e "FLUSH PRIVILEGES;"

		# Update local root password for better security
		mysql -u root --skip-password -e "ALTER USER 'root'@'localhost' IDENTIFIED BY '${DB_ROOT_PASSWORD}';"

		# Stop MariaDB cleanly after initialization
		mysqladmin -u root -p$DB_ROOT_PASSWORD shutdown

	else
		# If the database already exists, skip initialization
		echo "Inception: '${DB_DATABASE}' database already exists."
	fi
fi

# ---------------------------------------------------------------
#  Step 5: Start MariaDB in safe mode as the main process
# ---------------------------------------------------------------
exec "mysqld_safe"
