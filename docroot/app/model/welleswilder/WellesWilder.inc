<?php

/**
 * @file
 * Contains the WellesWilder class.
 */

/**
 * The Welles Wilder object.
 */
class WellesWilder implements WellesWilderInterface
{

	/**
	 * @var array $data
	 *  An array of market data with Welles Wilder data distinctions added to each
	 *  bar.
	 */
	protected $data = array();

	/**
	 * @var object $currentTrade
	 *  The current trade.
	 */
	protected $currentTrade;

	/**
	 * @var array $trades
	 *  An array of trade objects.
	 */
	protected $trades = array();

	/**
	 * @var integer $current
	 *  The index of the current bar.
	 */
	protected $current;

	/**
	 * Adds Welles Wilder's data distinctions to typical market data.
	 *
	 * @param array $data
	 *  An array of market data.
	 */
	public function __construct($data) {
		if (!is_array($data)) {
			throw new InvalidArgumentException('Invalid data.');
		}

		foreach ($data as $key => &$value) {
			$value['lop'] = $value['hip'] = FALSE;
			if (!isset($data[$key + 1]) || !isset($data[$key - 1])) {
				continue;
			}

			if ($value['low'] < $data[$key - 1]['low'] && $value['low'] < $data[$key + 1]['low']) {
				$value['lop'] = TRUE;
			}

			if ($value['high'] > $data[$key - 1]['high'] && $value['high'] > $data[$key + 1]['high']) {
				$value['hip'] = TRUE;
			}
		}
		$this->data = $data;
		$this->current = 0;
	}

	/**
	 * Iterate over the data. Test the strategy.
	 */
	public function crunch() {
		$len = count($this->data);
		for ($i = $this->current; $i < $len; $i++) {
			$this->interval($i);
		}
	}

	/**
	 * Executes one trading interval.
	 *
	 * @param array $prev
	 *  The previous intervals key in the data array.
	 * @param array $curr
	 *	The current intervals key in the data array.
	 */
	protected function interval($curr) {

	}

	/**
	 * Find the first trade.
	 */
	protected function setFirst() {

	}

	/**
	 * Get the trades.
	 *
	 * @return array
	 *  An array of the trades made during testing.
	 */
	public function getTrades() {
		return $this->trades;
	}

}