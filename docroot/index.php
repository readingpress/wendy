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

print '<h3>AUD/USD</h3>';
$data = new CSVData(WENDY_ROOT . '/data/AUDUSD_day.csv');
$ww = new ParabolicTimePriceSystem($data->getCSVData());
$ww->crunch();
$trades = $ww->getTrades();
$profit = 0;
$winners = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$profit += $trade->getProfit();
}
print 'Profit: $' . round($profit, 2) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';

print '<h3>EUR/USD</h3>';
$data = new CSVData(WENDY_ROOT . '/data/EURUSD_day.csv');
$ww = new ParabolicTimePriceSystem($data->getCSVData());
$ww->crunch();
$trades = $ww->getTrades();
$profit = 0;
$winners = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$profit += $trade->getProfit();
}
print 'Profit: $' . round($profit, 2) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';

print '<h3>NZD/USD</h3>';
$data = new CSVData(WENDY_ROOT . '/data/NZDUSD_day.csv');
$ww = new ParabolicTimePriceSystem($data->getCSVData());
$ww->crunch();
$trades = $ww->getTrades();
$profit = 0;
$winners = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$profit += $trade->getProfit();
}
print 'Profit: $' . round($profit, 2) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';

print '<h3>USD/CAD</h3>';
$data = new CSVData(WENDY_ROOT . '/data/USDCAD_day.csv');
$ww = new ParabolicTimePriceSystem($data->getCSVData());
$ww->crunch();
$trades = $ww->getTrades();
$profit = 0;
$winners = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$profit += $trade->getProfit();
}
print 'Profit: $' . round($profit, 2) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';

print '<h3>USD/CAD</h3>';
$data = new CSVData(WENDY_ROOT . '/data/USDCHF_day.csv');
$ww = new ParabolicTimePriceSystem($data->getCSVData());
$ww->crunch();
$trades = $ww->getTrades();
$profit = 0;
$winners = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$profit += $trade->getProfit();
}
print 'Profit: $' . round($profit, 2) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';