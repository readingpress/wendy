#!/bin/bash

# Configure a LAMP stack.
# We set DEBIAN_FRONTEND to noninteractive in order to bypass all prompts that
# require user input.
export DEBIAN_FRONTEND=noninteractive

# Ensure that downloads are the latest versions.
apt-get update

# Get Apache.
apt-get install apache2 -y

# Add a servername to httpd.conf in order to avoid Apache warning.
echo "ServerName localhost" | sudo tee /etc/apache2/httpd.conf

# Get MySQL and PHP.
apt-get install mysql-server php5 libapache2-mod-php5 php5-mysql -y

# Configure virtualhost for wendy.dev
cp /etc/apache2/sites-available/default /etc/apache2/sites-available/wendy.dev
sed -i 's/ServerAdmin webmaster@localhost/ServerAdmin michaeldewolf85@gmail.com\n        ServerName www.wendy.dev\n        ServerAlias wendy.dev/g' /etc/apache2/sites-available/wendy.dev
sed -i 's/DocumentRoot \/var\/www/DocumentRoot \/vagrant\/docroot/g' /etc/apache2/sites-available/wendy.dev
sed -i 's/<Directory \/var\/www\/>/<Directory \/vagrant\/docroot\/>/g' /etc/apache2/sites-available/wendy.dev
sed -i 's/AllowOverride None/AllowOverride All/>/g' /etc/apache2/sites-available/wendy.dev
a2ensite wendy.dev
a2enmod rewrite

# Restart apache so it updates its configuration.
service apache2 restart