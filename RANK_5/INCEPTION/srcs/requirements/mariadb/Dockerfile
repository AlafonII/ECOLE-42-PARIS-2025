# ---------------------------------------------------------------
#  MariaDB Dockerfile
#  This Dockerfile sets up a minimal MariaDB server environment
#  on Debian Bullseye with a custom initialization script.
# ---------------------------------------------------------------

# ---------------------------------------------------------------
#  Base image: Debian Bullseye
# ---------------------------------------------------------------
FROM debian:bullseye

# ---------------------------------------------------------------
#  Step 1: Install MariaDB server and clean up
# ---------------------------------------------------------------
# RUN apt-get -o Acquire::Check-Valid-Until=false -o Acquire::Check-Date=false update && \
# Update APT package lists, ignoring outdated timestamp validation
RUN apt-get update && \
# Install the MariaDB server package
    apt-get install -y mariadb-server && \
# Clean up downloaded package files to free space
    apt-get clean && \
# Remove unnecessary dependencies quietly and purge residual configuration files
    apt-get --yes --quiet autoremove --purge && \
# Delete cached APT lists to minimize the final image size
    rm -rf /var/lib/apt/lists/*

# ---------------------------------------------------------------
#  Step 2: Copy the initialization script into the container
# ---------------------------------------------------------------
# - Copy custom script to /usr/local/bin/
# - Set execution permissions (0755)
COPY --chmod=0755 ./tools/script.sh /usr/local/bin/

# ---------------------------------------------------------------
#  Step 3: Expose MariaDB default port
# ---------------------------------------------------------------
EXPOSE 3306

# ---------------------------------------------------------------
#  Step 4: Set the entrypoint to the initialization script
# ---------------------------------------------------------------
ENTRYPOINT ["/usr/local/bin/script.sh"]
