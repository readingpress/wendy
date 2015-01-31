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
$dms = new TrendBalancePointSystem($data->getCSVData());
$dms->crunch();
$trades = $dms->getTrades();

$profit = 0;
$winners = 0;
$w = 0;
$l = 0;
foreach($trades as $trade) {
	$p = $trade->profit();
	if ($p > 0) {
		$w += $p;
		$winners++;
	} else {
		$l += $p;
	}
	$profit += $trade->profit();
}
var_dump(round($winners / count($trades) * 100));
var_dump($profit / count($trades));
var_dump($w / $winners);
var_dump($l / (count($trades) - $winners));