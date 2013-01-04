<?php
/**
 * @file post.php
 *
 * The unified admin interface.
 * Steve Avery steve@theengineisred.com January 2013
 */

require '/inc/common.inc';
?>
<!DOCTYPE html>
<html class="version1">
	<head>
		<title>Gif Machine - Admin</title>
		<link rel="stylesheet" href="css/bootstrap.min.css" />
		<link rel="stylesheet" href="css/main.css" />
		<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
		<script src="/js/hoverIntent.js"></script>
		<script src="/js/dateFormat.js"></script>
	</head>
	<body>
		<header>
			<div class="navbar">
				<h2 class="inline"><?php print $page_title; ?></h2>
			</div>
		</header>
		<div role="main" class="main">
			<div class="list">
				<div class="blank frame">
				</div>
			</div>
		</div>
		<script src="/js/main.js"></script>
	</body>
</html>