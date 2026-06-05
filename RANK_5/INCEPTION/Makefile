################################################################################
#                                Project Structure                             #
################################################################################

NAME    := Inception
AUTHOR  := alafon
DATE    := $(shell date +"%Y/%m/%d %T")

ENV_FILE ?= srcs/.env
DOCKER_COMPOSE ?= srcs/docker-compose.yml
ENV_FILE_ABS := $(abspath $(ENV_FILE))

################################################################################
#                                  Makefile BASIC COMMANDS                     #
################################################################################

.DEFAULT_GOAL := all

up: header build

setup: header
	@bash -c '. $(ENV_FILE_ABS); mkdir -p "/home/alafon/data/website" "/home/alafon/data/database"'
	ENV_FILE=$(ENV_FILE_ABS) bash ./srcs/requirements/tools/setup.sh

build: header setup
	docker compose -f $(DOCKER_COMPOSE) --env-file $(ENV_FILE) up -d --build

start: header
	docker compose -f $(DOCKER_COMPOSE) start

stop: header
	docker compose -f $(DOCKER_COMPOSE) stop

down: header
	docker compose -f $(DOCKER_COMPOSE) down

clean: header stop
	docker compose -f $(DOCKER_COMPOSE) down

fclean: header clean
	docker compose -f $(DOCKER_COMPOSE) down --remove-orphans
	docker compose -f $(DOCKER_COMPOSE) down --volumes

re: header fclean all


################################################################################
#                                TEST & DEBUG COMMANDS                         #
################################################################################
################################################################################
#                                🧪 TEST & DEBUG COMMANDS                      #
################################################################################

# Show all currently running Docker containers related to the project
ps:
	@echo "🔍 Checking running containers..."
	@docker ps
	@echo ""
	@echo "✅ Expected: nginx, wordpress, and mariadb should all appear as 'Up'."
	@echo ""

# List all Docker images currently available on the system
# Useful for checking which images were built or left behind
images:
	@echo "🔍 Listing all Docker images..."
	@docker images --format "table {{.Repository}}\t{{.Tag}}\t{{.CreatedAt}}\t{{.Size}}"
	@echo ""
	@echo "✅ Expected: one image each for nginx, wordpress, and mariadb."
	@echo ""

# Display all Docker networks
# Helps confirm that the project’s bridge network (inception-network) exists
network:
	@echo "🔍 Checking Docker networks..."
	@docker network ls
	@echo ""
	@echo "✅ Expected: inception-network should exist and be of type 'bridge'."
	@echo ""

# List all Docker volumes
# Ensures that the WordPress and MariaDB bind mounts or volumes are created and attached
volume:
	@echo "🔍 Listing Docker volumes..."
	@docker volume ls
	@echo ""
	@echo "✅ Expected: volumes or bind mounts for database and website should exist."
	@echo ""

# Stream live logs from the Nginx container
# Helpful for monitoring incoming requests and debugging web server issues
logs:
	@echo "🔍 Streaming live logs from Nginx..."
	@echo "   (Press Ctrl+C to stop viewing logs)"
	@docker logs -t -f $$(docker compose -f $(DOCKER_COMPOSE) ps -q nginx)

# Test website reachability for HTTPS and HTTP
# - HTTPS should return status 200 (OK)
# - HTTP should be blocked or redirected (as per TLS-only requirement)
curl:
	@echo "🔍 Testing connectivity to https://alafon.42.fr"
	@if curl -k -s -o /dev/null -w "%{http_code}" https://alafon.42.fr | grep -q "200"; then \
		echo "✅ HTTPS connection successful — your site is reachable and TLS works."; \
	else \
		echo "❌ HTTPS connection failed — check Nginx, certificates, or container status."; \
	fi
	@echo ""
	@echo "🔍 Testing connectivity to http://alafon.42.fr"
	@if curl -s -o /dev/null -w "%{http_code}" http://alafon.42.fr | grep -q "200"; then \
		echo "⚠️  HTTP connection unexpectedly succeeded — it should redirect or be blocked."; \
	else \
		echo "✅ HTTP connection is blocked or redirected — Nginx TLS-only setup works."; \
	fi
	@echo ""

# Check if the MariaDB container is healthy and accepting connections
dbtest:
	@echo "🔍 Testing MariaDB container health..."
	@bash -c '. $(ENV_FILE_ABS); docker exec mariadb mariadb -h$$DB_HOST -u$$DB_USER -p$$DB_USER_PASSWORD -e "SELECT 1;" > /dev/null 2>&1 && \
	echo "✅ MariaDB connection successful — database responding properly." || \
	echo "❌ Failed to connect to MariaDB — check credentials or container status."'
	@echo ""


# Verify WordPress files and permissions inside the container
wptest:
	@echo "🔍 Checking WordPress directory and configuration..."
	@docker exec wordpress bash -c 'test -f /var/www/html/wordpress/wp-config.php && echo "✅ wp-config.php found." || echo "❌ wp-config.php missing!"'
	@docker exec wordpress bash -c 'wp core is-installed --path=/var/www/html/wordpress --allow-root > /dev/null 2>&1 && echo "✅ WordPress core is installed." || echo "❌ WordPress core not installed."'
	@echo ""


# Confirm that all containers share the same network
nettest:
	@echo "🔍 Checking container network attachments..."
	@docker network inspect inception-network --format='{{json .Containers}}' | grep -E '"Name":"(nginx|wordpress|mariadb)"' > /dev/null && \
	echo "✅ All containers are attached to inception-network." || \
	echo "❌ One or more containers are missing from inception-network."
	@echo ""

# Check disk usage of Docker resources
disk:
	@echo "🔍 Checking Docker disk usage..."
	@docker system df
	@echo ""
	@echo "✅ Review total disk space used by images, containers, and volumes."
	@echo ""

################################################################################
#                              🩺 DOCTOR COMMAND                               #
################################################################################

doctor: header
	@clear
	@echo ""
	@echo "🩺==============================================🩺"
	@echo "           INCEPTION PROJECT HEALTH CHECK"
	@echo "🩺==============================================🩺"
	@echo ""

	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "🔹 [1/8] Checking Running Containers (ps)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@$(MAKE) --no-print-directory ps

	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "🔹 [2/8] Checking Docker Networks (network)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@$(MAKE) --no-print-directory network

	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "🔹 [3/8] Checking Docker Volumes (volume)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@$(MAKE) --no-print-directory volume

	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "🔹 [4/8] Verifying Network Attachments (nettest)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@$(MAKE) --no-print-directory nettest

	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "🔹 [5/8] Testing Database Connectivity (dbtest)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@$(MAKE) --no-print-directory dbtest

	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "🔹 [6/8] Checking WordPress Configuration (wptest)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@$(MAKE) --no-print-directory wptest

	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "🔹 [7/8] Testing HTTPS / HTTP Connectivity (curl)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@$(MAKE) --no-print-directory curl

	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "🔹 [8/8] Checking Docker Disk Usage (disk)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@$(MAKE) --no-print-directory disk

	@echo ""
	@echo "🩺==============================================🩺"
	@echo "             ✅ All checks completed!"
	@echo "🩺==============================================🩺"
	@echo ""


.PHONY: header all build start stop down ps clean fclean re images network volume logs setup

################################################################################
#                                  Makefile visual                             #
################################################################################

COM_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

COM_STRING  = "running"

define run_and_test
		printf "%b%-46b" "$(COM_COLOR)$(COM_STRING) " "$(OBJ_COLOR)$(@F)$(NO_COLOR)"; \
		$(1); \
		RESULT=$$?; \
		if [ $$RESULT -ne 0 ]; then \
			printf "%b\n" "$(ERROR_COLOR)[✖]$(NO_COLOR)"; \
		else  \
			printf "%b\n" "$(OK_COLOR)[✓]$(NO_COLOR)"; \
		fi; \
		exit $$RESULT
endef

header:
	@printf "%b" "$(OBJ_COLOR)"
	@echo "                          "
	@echo "        :::      :::::::: "
	@echo "      :+:      :+:    :+: "
	@echo "    +:+ +:+         +:+   "
	@echo "  +#+  +:+       +#+      "
	@echo "+#+#+#+#+#+   +#+         "
	@echo "     #+#    #+#           "
	@echo "    ###   ########.Inception  "
	@printf "%b" "$(NO_COLOR)"
	@echo
	@printf "%b" "$(OK_COLOR)"
	@printf "%b" "$(OBJ_COLOR)Name:   $(WARN_COLOR)$(NAME)\n"
	@printf "%b" "$(OBJ_COLOR)Author: $(WARN_COLOR)$(AUTHOR)\n"
	@printf "%b" "$(OBJ_COLOR)Date:   $(WARN_COLOR)$(DATE)\n"
	@echo
	@printf "%b" "$(NO_COLOR)"
