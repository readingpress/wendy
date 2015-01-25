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
$largest = 0;
$max = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$p = $trade->getProfit();
	$largest = $p > $largest ? $p : $largest;
	$profit += $p;
	$max += $trade->getMax();
}
print 'Total Trades: ' . count($trades) . '<br />';
print 'Total Profit: $' . round($profit, 4) . '<br />';
print 'Largest Profit: $' . round($largest, 4) . '</br>';
print 'Average Profit: $' . round($profit / count($trades), 4) . '</br>';
print 'Total Highs: $' . round($max, 4) . '<br />';
print 'Average High: $' . round($max / count($trades), 4) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';

print '<h3>EUR/USD</h3>';
$data = new CSVData(WENDY_ROOT . '/data/EURUSD_day.csv');
$ww = new ParabolicTimePriceSystem($data->getCSVData());
$ww->crunch();
$trades = $ww->getTrades();
$profit = 0;
$winners = 0;
$max = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$p = $trade->getProfit();
	$largest = $p > $largest ? $p : $largest;
	$profit += $p;
	$max += $trade->getMax();
}
print 'Total Trades: ' . count($trades) . '<br />';
print 'Total Profit: $' . round($profit, 4) . '<br />';
print 'Largest Profit: $' . round($largest, 4) . '</br>';
print 'Average Profit: $' . round($profit / count($trades), 4) . '</br>';
print 'Total Highs: $' . round($max, 4) . '<br />';
print 'Average High: $' . round($max / count($trades), 4) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';

print '<h3>NZD/USD</h3>';
$data = new CSVData(WENDY_ROOT . '/data/NZDUSD_day.csv');
$ww = new ParabolicTimePriceSystem($data->getCSVData());
$ww->crunch();
$trades = $ww->getTrades();
$profit = 0;
$winners = 0;
$max = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$p = $trade->getProfit();
	$largest = $p > $largest ? $p : $largest;
	$profit += $p;
	$max += $trade->getMax();
}
print 'Total Trades: ' . count($trades) . '<br />';
print 'Total Profit: $' . round($profit, 4) . '<br />';
print 'Largest Profit: $' . round($largest, 4) . '</br>';
print 'Average Profit: $' . round($profit / count($trades), 4) . '</br>';
print 'Total Highs: $' . round($max, 4) . '<br />';
print 'Average High: $' . round($max / count($trades), 4) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';

print '<h3>USD/CAD</h3>';
$data = new CSVData(WENDY_ROOT . '/data/USDCAD_day.csv');
$ww = new ParabolicTimePriceSystem($data->getCSVData());
$ww->crunch();
$trades = $ww->getTrades();
$profit = 0;
$winners = 0;
$max = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$p = $trade->getProfit();
	$largest = $p > $largest ? $p : $largest;
	$profit += $p;
	$max += $trade->getMax();
}
print 'Total Trades: ' . count($trades) . '<br />';
print 'Total Profit: $' . round($profit, 4) . '<br />';
print 'Largest Profit: $' . round($largest, 4) . '</br>';
print 'Average Profit: $' . round($profit / count($trades), 4) . '</br>';
print 'Total Highs: $' . round($max, 4) . '<br />';
print 'Average High: $' . round($max / count($trades), 4) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';

print '<h3>USD/CAD</h3>';
$data = new CSVData(WENDY_ROOT . '/data/USDCHF_day.csv');
$ww = new ParabolicTimePriceSystem($data->getCSVData());
$ww->crunch();
$trades = $ww->getTrades();
$profit = 0;
$winners = 0;
$max = 0;
foreach ($trades as $trade) {
	if ($trade->winner()) {
		$winners++;
	}
	$p = $trade->getProfit();
	$largest = $p > $largest ? $p : $largest;
	$profit += $p;
	$max += $trade->getMax();
}
print 'Total Trades: ' . count($trades) . '<br />';
print 'Total Profit: $' . round($profit, 4) . '<br />';
print 'Largest Profit: $' . round($largest, 4) . '</br>';
print 'Average Profit: $' . round($profit / count($trades), 4) . '</br>';
print 'Total Highs: $' . round($max, 4) . '<br />';
print 'Average High: $' . round($max / count($trades), 4) . '<br />';
print 'Success Rate: ' . round($winners / count($trades) * 100) . '%<br />';