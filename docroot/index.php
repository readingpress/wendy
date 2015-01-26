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
$dms->crunch();
$trades = $dms->getTrades();

$profit = 0;
$winners = 0;
$account = 2000;
$spread = .0004;
$leverage = 50;
foreach ($trades as $trade) {
	$tp = $trade->profit();
	$profit += $tp;
	$ac_profit = ($account / 10) * ($tp - $spread) * 50;
	$account += $ac_profit;
	if ($tp > 0) {
		$winners++;
	}
}

print 'Total trades: ' . count($trades) . '<br />';
print 'Total profit: ' . number_format($profit, 4) . '<br />';
print 'Average profit: ' . number_format($profit / count($trades), 4) . '<br />';
print 'Success rate: ' . round($winners / count($trades) * 100) . '%<br />';
var_dump($account);