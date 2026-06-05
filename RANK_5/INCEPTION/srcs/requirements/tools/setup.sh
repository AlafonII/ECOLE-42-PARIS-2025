#!/bin/bash

# ---------------------------------------------------------------
#  setup.sh — Local domain configuration
#  This script updates /etc/hosts to point a chosen domain name
#  (e.g., alafon.42.fr) to localhost (127.0.0.1)
# ---------------------------------------------------------------

# ---------------------------------------------------------------
#  Step 1: Define target
# ---------------------------------------------------------------
TARGET='/etc/hosts'       # System hosts file to be modified

# ---------------------------------------------------------------
#  Step 2: Locate the .env file
# ---------------------------------------------------------------
# Locate the .env file
ENV_FILE="${ENV_FILE:-$(dirname "$0")/../../.env}"

if [ -f "$ENV_FILE" ]; then # Load environment variables from .env if it exists
    set -a					# Automatically export all sourced vars
    . "$ENV_FILE"			# Source the .env file
    set +a					# Stop automatic export
else
    echo "Warning: .env file not found at $ENV_FILE"
fi

# ---------------------------------------------------------------
#  Step 3: Determine the domain name to use
# ---------------------------------------------------------------
# Priority: DOMAIN_NAME (cleaned) → LOGIN.42.fr → alafon.42.fr

# If DOMAIN_NAME is set, use it. Otherwise, fall back to "<LOGIN>.42.fr"
DOMAIN="${DOMAIN_NAME:-${LOGIN}.42.fr}"

# If both DOMAIN_NAME and LOGIN are empty, default to "alafon.42.fr"
DOMAIN="${DOMAIN:-alafon.42.fr}"

# Remove "https://" from the beginning of the domain (if present)
DOMAIN="${DOMAIN#https://}"

# Remove "http://" from the beginning of the domain (if present)
DOMAIN="${DOMAIN#http://}"


# ---------------------------------------------------------------
#  Step 4: Check current /etc/hosts entries
# ---------------------------------------------------------------
# Check if /etc/hosts already contains the domain
if ! grep -q "$DOMAIN" "$TARGET"; then
    echo "Adding domain mapping to /etc/hosts..."
    sudo sed -i "1s/localhost/${DOMAIN}/" "$TARGET"
else
    echo "Domain already present in /etc/hosts"
fi

# ---------------------------------------------------------------
#  Step 5: Display confirmation if domain was correctly applied
# ---------------------------------------------------------------
if grep -q "$DOMAIN" "$TARGET"; then
	echo "✅ Domain [ok]: $DOMAIN"
else
	echo "❌ Domain not found in /etc/hosts"
fi
