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
print 'Total Profit: $' . number_format($profit, 4) . '<br />';
print 'Largest Profit: $' . number_format($largest, 4) . '</br>';
print 'Average Profit: $' . number_format($profit / count($trades), 4) . '</br>';
print 'Total Highs: $' . number_format($max, 4) . '<br />';
print 'Average High: $' . number_format($max / count($trades), 4) . '<br />';
print 'Success Rate: ' . number_format($winners / count($trades) * 100) . '%<br />';

$account = 2000;
foreach ($trades as $trade) {
	$chunk = $account / 10;
	$profit = $chunk * number_format($trade->getProfit() - .0004, 4) * 50;
	$account += $profit;
}

print 'Account starting at $2000 trading 10% on each trade, with 50 leverage.<br />';
print '$' . number_format($account, 2);