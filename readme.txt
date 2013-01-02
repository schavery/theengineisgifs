Gif Machine

This suite is under development for the remote display and administration of gifs.
The project was developed to display a gif in the window of a shop, so that passers-by would like us better.

Requirements:
This code is not built with portability in mind. The host machine needs to have apache, php, mysql, imagemagick, and node.js installed. I am not sure what versions are required. My host uses:
php 5.4.4 / apache 2.4.2 / mysql 5.5.25a / xampp 1.8.0
imagick 6.7.9 Q16 32bit
node.js 0.8.14

The client can be any browser, in my particular set up I use a old box with Chrome configured to launch in kiosk mode.

Usage:
You'll need to set up a host with your apache configuration. Visit the base URL to see the current gif to be displayed, fullscreen. You'll also need to add a mysql user.
To administer your gifs, visit base/admin.php.

You can submit a gif from this page, or visit the queue to see a list of the gifs that have been submitted and select ones to display.
