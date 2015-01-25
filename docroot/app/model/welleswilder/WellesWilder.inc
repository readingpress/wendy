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
		$this->data = $data;
		$this->current = 0;
		$this->getExtremes();
		$this->getTrueRange();
		$this->addDM();
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
	 * Processes one trading interval.
	 *
	 * @param integer $index
	 *  The index of the next bar.
	 */
	protected function interval($index) {
		if (!is_numeric($index)) {
			throw new InvalidArgumentException('Invalid index.');
		}
		// We are on the last item in the data array. We can't continue.
		if (!isset($this->data[$index + 1])) {
			return;
		}
		$intMethod = "{$this->currentTrade->getDirection()}Interval";
		$this->$intMethod($this->data[$index + 1], $this->data[$index]);
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

	/**
	 * Crunches an interval for a short trade.
	 *
	 * @param array $curr
	 *  The current bar.
	 * @param array $prev
	 *  The previous bar.
	 */
	protected function shortInterval($curr, $prev) {

	}

	/**
	 * Crunches an interval for a long trade.
	 *
	 * @param array $curr
	 *  The current bar.
	 * @param array $prev
	 *  The previous bar.
	 */
	protected function longInterval($curr, $prev) {

	}

	/**
	 * Get high points and low points and add them as properties to the trading
	 * data.
	 */
	private function getExtremes() {
		foreach ($this->data as $key => &$value) {
			$value['lop'] = $value['hip'] = FALSE;
			if (!isset($this->data[$key + 1]) || !isset($this->data[$key - 1])) {
				continue;
			}

			if ($value['low'] < $this->data[$key - 1]['low'] && $value['low'] < $this->data[$key + 1]['low']) {
				$value['lop'] = TRUE;
			}

			if ($value['high'] > $this->data[$key - 1]['high'] && $value['high'] > $this->data[$key + 1]['high']) {
				$value['hip'] = TRUE;
			}
		}
	}

	private function getTrueRange() {
		foreach ($this->data as $key => &$value) {
			$r1 = $value['high'] - $value['low'];
			$r2 = isset($this->data[$key - 1]) ? $value['high'] - $this->data[$key - 1]['close'] : 0;
			$r3 = isset($this->data[$key - 1]) ? $this->data[$key - 1]['close'] - $value['low'] : 0;
			$value['true_range'] = max(array($r1, $r2, $r3));
			if ($key - 13 < 0) {
				$value['volatility_index'] = FALSE;
				continue;
			}
			if ($key - 13 == 0) {
				$trsum = 0;
				for ($i = 0; $i <= $key; $i++) {
					$trsum += $this->data[$i]['true_range'];
				}
				$value['volatility_index'] = $trsum / 14;
				continue;
			}

			$value['volatility_index'] = (13 * $this->data[$key - 1]['volatility_index'] + $value['true_range']) / 14;
		}
	}

	private function addDM() {
		foreach ($this->data as $key => &$value) {
			if ($key == 0) {
				continue;
			}
			$plus_dm = $value['high'] - $this->data[$key - 1]['high'] > 0 ? $value['high'] - $this->data[$key - 1]['high'] : 0;
			$minus_dm = $this->data[$key - 1]['low'] - $value['low'] > 0 ? $this->data[$key - 1]['low'] - $value['low'] : 0;
			$value['plus_di'] = $value['true_range'] > 0 ? $plus_dm / $value['true_range'] : 0;
			$value['minus_di'] = $value['true_range'] > 0 ? $minus_dm / $value['true_range'] : 0;

		}
	}

}