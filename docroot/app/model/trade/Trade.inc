<?php

/**
 * @file
 * Contains the Trade class.
 */

/**
 * The trade object.
 */
class Trade implements TradeInterface
{

	/**
	 * @var string $dir
	 *  The direction of the trade.
	 */
	private $dir;

	/**
	 * @var float $entrance
	 *  The entrance price.
	 */
	private $entrance;

	/**
	 * @var array $open
	 *  An array of data representing the opening bar.
	 */
	private $open;

	/**
	 * @var float $exit
	 *  The exit price.
	 */
	private $exit;

	/**
	 * @var array $close
	 *  An array of data representing the closing bar.
	 */
	private $close;

	/**
	 * @var float $high
	 *  The highest price reached while in this trade.
	 */
	private $high;

	/**
	 * @var float $low
	 *  The lowest price reached while in this trade.
	 */
	private $low;

	/**
	 * @var float $profit
	 *  The total dollar amount gained/losed in this trade.
	 */
	private $profit;

	/**
	 * @var boolean $winner
	 *  Whether or not this trade made money.
	 */
	private $winner;

	/**
	 * @var boolean $max
	 *  The maximum profit that the trade had during its lifetime.
	 */
	private $max;

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
	public function __construct($dir, $entrance, $open) {
		$this->dir = $dir;
		$this->open = $open;
		$this->entrance = $entrance;
		$this->high = 0;
		$this->low = 100000000000;
		$this->profit = 0;
		$this->winner = FALSE;
		$this->max = 0;
	}

	/**
	 * Crunch a bar and update the trade, mostly used just to update the high and 
	 * low.
	 *
	 * @param array $bar
	 *  An associative array of data representing a single bar.
	 */
	public function crunchBar($bar) {
		$this->high = max(array($this->high, $bar['high']));
		$this->low = min(array($this->low, $bar['low']));
	}

	/**
	 * Get the direction of the trade.
	 *
	 * @return string
	 * 	The direction of the trade, either "short" or "long".
	 */
	public function getDirection() {
		return $this->dir;
	}

	/**
	 * Get the high for this trade.
	 *
	 * @return float
	 *  The high for the trade.
	 */
	public function getHigh() {
		return $this->high;
	}

	/**
	 * Get the low for this trade.
	 *
	 * @return float
	 *  The low for the trade.
	 */
	public function getLow() {
		return $this->low;
	}

	/**
	 * Close the trade.
	 *
	 * @param float $exit
	 *  The exit price.
	 * @param array $bar
	 *  An array of data representing the closing bar of the trade.
	 */
	public function close($exit, $bar) {
		$this->exit = $exit;
		$this->close = $bar;
		$this->calculateProfit();
	}

	/**
	 * Return the profit on this trade.
	 *
	 * @return float
	 *  The total dollar amount gained/lost on this trade.
	 */
	public function getProfit() {
		return $this->profit;
	}

	/**
	 * Return whether the trade made money.
	 *
	 * @return boolean
	 *  Whether or not this trade made money.
	 */
	public function winner() {
		return $this->winner;
	}

	/**
	 * Return the maximum profit that the trade had during its lifetime.
	 *
	 * @return float
	 *  The maximum profit that the trade had during its lifetime.
	 */
	public function getMax() {
		return $this->max;
	}

	/**
	 * Return the dollar amount gained/lossed for this trade.
	 *
	 * @return float
	 *  The total dollar amount.
	 */
	private function calculateProfit() {
		switch ($this->dir) {
			case 'short':
				if ($this->entrance > $this->exit) {
					$this->winner = TRUE;
				}
				$this->max = $this->entrance - $this->low;
				$this->profit = $this->entrance - $this->exit;
				break;
			case 'long':
				if ($this->entrance < $this->exit) {
					$this->winner = TRUE;
				}
				$this->max = $this->high - $this->entrance;
				$this->profit = $this->exit - $this->entrance;
				break;
		}
	}

}