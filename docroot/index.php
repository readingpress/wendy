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

$data = new CSVData(WENDY_ROOT . '/data/AUDUSD_day.csv');
print '<pre>';
$ww = new WellesWilder($data->getCSVData());
