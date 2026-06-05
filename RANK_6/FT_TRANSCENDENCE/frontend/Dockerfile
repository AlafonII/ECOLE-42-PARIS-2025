FROM node:20-alpine AS builder
WORKDIR /app

# Copy package files and install dependencies
COPY package*.json ./
RUN npm install

COPY . .
RUN npm run build

# serve the built files with Nginx over HTTPS
FROM nginx:1.27-alpine

RUN apk add --no-cache openssl

RUN mkdir -p /etc/nginx/certs && \
    openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
    -keyout /etc/nginx/certs/key.pem -out /etc/nginx/certs/cert.pem \
    -subj "/CN=ft_transcendence" -addext "subjectAltName=DNS:ft_transcendence"

# handles HTTPS and SPA routing
COPY nginx.conf /etc/nginx/conf.d/default.conf

# Copy the built frontend from the builder stage into Nginx's public folder
COPY --from=builder /app/dist /usr/share/nginx/html

EXPOSE 5173

CMD ["nginx", "-g", "daemon off;"]
