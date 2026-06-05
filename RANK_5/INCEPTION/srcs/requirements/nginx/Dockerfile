# ---------------------------------------------------------------
#  Nginx Dockerfile for the Inception project
#  This Dockerfile sets up an Nginx web server with SSL support
#  using a self-signed certificate.
# ---------------------------------------------------------------

# ---------------------------------------------------------------
#  Base image: Debian Bullseye
# ---------------------------------------------------------------
FROM debian:bullseye

# ---------------------------------------------------------------
#  Step 1: Install required packages
# ---------------------------------------------------------------
# RUN apt-get -o Acquire::Check-Valid-Until=false -o Acquire::Check-Date=false update && \
# Update package lists, ignoring outdated timestamp validation
RUN apt-get update && \
# Install the ping utility (iputils-ping) for basic network diagnostics
    apt install -y iputils-ping && \
# Install the Nginx web server and OpenSSL for generating SSL certificates
    apt-get install -y nginx openssl && \
# Remove cached APT package lists to reduce the final image size
    rm -rf /var/lib/apt/lists/*

# ---------------------------------------------------------------
#  Step 2: Copy Nginx configuration file
# ---------------------------------------------------------------
# - Copy the custom site configuration into the container
# - Store it in the sites-available directory
COPY ./conf/default.conf /etc/nginx/sites-available/alafon.42.fr.conf

# ---------------------------------------------------------------
#  Step 3: Generate a self-signed SSL certificate
# ---------------------------------------------------------------
# Create a new self-signed X.509 certificate request
RUN openssl req -x509 \
# Use SHA-256 for the certificate’s hashing algorithm
    -sha256 \
# Disable password protection on the private key (no passphrase prompt)
    -nodes \
# Set the certificate validity period to 365 days
    -days 365 \
# Generate a new 2048-bit RSA private key
    -newkey rsa:2048 \
# Save the private key to the specified path inside the container
    -keyout /etc/ssl/private/nginx-selfsigned.key \
# Save the generated public certificate to the given location
    -out /etc/ssl/certs/nginx-selfsigned.crt \
# Define certificate subject information: country, state, location, org, unit, and domain
    -subj "/C=FR/ST=Ile-de-France/L=Paris/O=42/OU=alafon42/CN=alafon.42.fr/"


# ---------------------------------------------------------------
#  Step 4: Enable the site configuration
# ---------------------------------------------------------------
# - Create a symbolic link from sites-available to sites-enabled
RUN ln -s /etc/nginx/sites-available/alafon.42.fr.conf /etc/nginx/sites-enabled/alafon.42.fr.conf

# ---------------------------------------------------------------
#  Step 5: Expose HTTPS port
# ---------------------------------------------------------------
EXPOSE 443

# ---------------------------------------------------------------
#  Step 6: Define stop signal for shutdown
# ---------------------------------------------------------------
STOPSIGNAL SIGQUIT

# ---------------------------------------------------------------
#  Step 7: Launch Nginx in the foreground (container main process)
# ---------------------------------------------------------------
CMD ["nginx", "-g", "daemon off;"]
