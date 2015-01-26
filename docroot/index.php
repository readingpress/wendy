<?php

/**
 * @file
 * The PHP page that serves all page requests on a Wendy installation. The 
 * routines here dispatch control to the appropriate handler.
 */

error_reporting(-1);
ini_set('display_errors', 'On');

/**
 * Root directory of the Wendy installation.
 */
define('WENDY_ROOT', getcwd());
require_once WENDY_ROOT . '/app/bootstrap.php';

print '<pre>';

$data = new CSVData(WENDY_ROOT . '/data/AUDUSD_day.csv');
$dms = new DirectionalMovementSystem($data->getCSVData());
var_dump($dms->getData());