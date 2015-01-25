<?php

/**
 * @file
 * Contains the TradeInterface.
 */

/**
 * The TradeInterface.
 */
interface TradeInterface
{

	/**
	 * Set up basic trade properties
	 *
	 * @param string $dir
	 *  The direction of the trade, either "short" or "long".
	 * @param float $entrance
	 *  The entrance price.
	 * @param array $open
	 *  An array of data representing the opening bar of the trade.
	 */
	public function __construct($dir, $entrance, $open);

	/**
	 * Crunch a bar and update the trade, mostly used just to update the high and 
	 * low.
	 *
	 * @param array $bar
	 *  An associative array of data representing a single bar.
	 */
	public function crunchBar($bar);

	/**
	 * Get the direction of the trade.
	 *
	 * @return string
	 * 	The direction of the trade, either "short" or "long".
	 */
	public function getDirection();

	/**
	 * Get the high for this trade.
	 *
	 * @return float
	 *  The high for the trade.
	 */
	public function getHigh();

	/**
	 * Get the low for this trade.
	 *
	 * @return float
	 *  The low for the trade.
	 */
	public function getLow();

	/**
	 * Return the profit on this trade.
	 *
	 * @return float
	 *  The total dollar amount gained/lost on this trade.
	 */
	public function getProfit();
	
	/**
	 * Return whether the trade made money.
	 *
	 * @return boolean
	 *  Whether or not this trade made money.
	 */
	public function winner();

}