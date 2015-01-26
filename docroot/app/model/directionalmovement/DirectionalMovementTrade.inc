<?php

/**
 * @file
 * Contains the DirectionalMovementTrade class.
 */

class DirectionalMovementTrade
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
		$this->setStop();
	}

	/**
	 * Get the stop for a trade.
	 *
	 * @return integer
	 *  The stop for this trade.
	 */
	public function getStop() {
		return $this->stop;
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
	private function setStop() {
		switch ($this->dir) {
			case 'short':
				$this->stop = $this->open['high'];
				break;
			case 'long':
				$this->stop = $this->open['low'];
				break;
		}
	}

}