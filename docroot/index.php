<?php

/**
 * @file
 * The PHP page that serves all page requests on a Wendy installation. The 
 * routines here dispatch control to the appropriate handler.
 */

/**
 * Root directory of the Wendy installation.
 */
define('WENDY_ROOT', getcwd());
require_once WENDY_ROOT . '/app/bootstrap.php';
?>

<!DOCTYPE html>
<html>
	<head>
	</head>
	<body>
		<h1>Wendy</h1>
		<p>Welcome to Wendy.</p>
	</body>
</html>