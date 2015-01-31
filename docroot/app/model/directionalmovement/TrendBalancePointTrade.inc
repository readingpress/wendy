<?php

/**
 * @file
 * Contains the TrendBalancePointTrade class.
 */

class TrendBalancePointTrade
{

	/**
	 * @var integer $entrance
	 *  The opening price.
	 */
	private $entrance;

	/**
	 * @var integer $exit
	 *  The closing price.
	 */
	private $exit;

	/**
	 * @var array $open
	 *  The opening bar.
	 */
	private $open;

	/**
	 * @var array $close
	 *  The closing bar.
	 */
	private $close;

	/**
	 * @var string $dir
	 *  The direction of the trade, either "short" or "long".
	 */
	private $dir;

	/**
	 * @var integer $stop
	 *  The stop on the trade.
	 */
	private $stop;

	/**
	 * Instantiate the trade.
	 *
	 * @param integer $price
	 *  The opening price.
	 * @param string $dir
	 *  The direction of the trade, either "short" or "long".
	 * @param array $bar
	 *  An array of data from the opening bar.
	 */
	public function __construct($price, $dir, $bar) {
		$this->entrance = $price;
		$this->open = $bar;
		$this->dir = $dir;
	}

	/**
	 * Returns the direction of the current trade.
	 *
	 * @return string
	 *  The direction of the current trade.
	 */
	public function getDir() {
		return $this->dir;
	}

	/**
	 * Returns whether the stop for the trade has been hit in a bar.
	 *
	 * @param array $bar
	 *  The bar to check.
	 *
	 * @return mixed
	 *  The stop value, if it was hit, FALSE otherwise.
	 */
	public function stop($bar) {
		if ($this->stop > $bar['low'] && $this->stop < $bar['high']) {
			return $this->stop;
		}
		return FALSE;
	}

	/**
	 * Returns whether the take profit for the trade has been hit in a bar.
	 *
	 * @param array $bar
	 *  The bar to check.
	 *
	 * @return mixed
	 *  The stop value, if it was hit, FALSE otherwise.
	 */
	public function takeProfit($bar) {
		if ($this->takeProfit > $bar['low'] && $this->takeProfit < $bar['high']) {
			return $this->takeProfit;
		}
		return FALSE;
	}

	/**
	 * Get the return on the trade.
	 */
	public function profit() {
		$profit = 0;
		switch ($this->dir) {
			case 'short':
				$profit = $this->entrance - $this->exit;
				break;
			case 'long':
				$profit = $this->exit - $this->entrance;
				break;
		}
		return $profit;
	}

	/**
	 * Close the trade.
	 *
	 * @param integer $price
	 *  The closing price.
	 * @param array $bar
	 *  The closing bar.
	 */
	public function close($price, $bar) {
		$this->exit = $price;
		$this->close = $bar;
	}

	/**
	 * Set the stop for a trade. 
	 */
	public function setStop($bar) {
		switch ($this->dir) {
			case 'short':
				$this->stop = $bar['x'] + $bar['tr_1'];
				break;
			case 'long':
				$this->stop = $bar['x'] - $bar['tr_1'];
				break;
		}
	}

	/**
	 * Set the take profit for a trade. 
	 */
	public function setTakeProfit($bar) {
		switch ($this->dir) {
			case 'short':
				$this->takeProfit = 2 * $bar['x'] - $bar['high'];
				break;
			case 'long':
				$this->takeProfit = 2 * $bar['x'] - $bar['low'];
				break;
		}
	}

}