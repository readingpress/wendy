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
			$value['plus_dm'] = $value['minus_dm'] = $value['dm_14_plus'] = $value['dm_14_minus'] = $value['tr_14_sum'] = $value['di_plus'] = $value['di_minus'] = $value['di_difference'] = $value['di_sum'] = $value['dx'] = $value['adx'] = $value['adxr'] = 0;
			if ($key == 0) {
				continue;
			}
			$value['plus_dm'] = $value['high'] - $this->data[$key - 1]['high'] > 0 ? $value['high'] - $this->data[$key - 1]['high'] : 0;
			$value['minus_dm'] = $this->data[$key - 1]['low'] - $value['low'] > 0 ? $this->data[$key - 1]['low'] - $value['low'] : 0;
			if ($key - 13 < 0) {
				$value['directional_indicator_plus'] = FALSE;
				$value['directional_indicator_minus'] = FALSE;
				continue;
			}
			if ($key - 13 == 0) {
				$di_plus_sum = 0;
				$di_minus_sum = 0;
				$tr_sum = 0;
				for ($i = 0; $i <= $key; $i++) {
					$tr_sum += $this->data[$i]['true_range'];
					$di_plus_sum += $this->data[$i]['plus_dm'];
					$di_minus_sum += $this->data[$i]['minus_dm'];
				}
				$value['dm_14_plus'] = $di_plus_sum;
				$value['dm_14_minus'] = $di_minus_sum;
				$value['tr_14_sum'] = $tr_sum;
				$value['di_plus'] = $di_plus_sum / $tr_sum;
				$value['di_minus'] = $di_minus_sum / $tr_sum;
				$value['di_difference'] = abs($value['di_plus'] - $value['di_minus']);
				$value['di_sum'] = $value['di_plus'] + $value['di_minus'];
				continue;
			}

			$yest_dm_plus = $this->data[$key - 1]['dm_14_plus'];
			$value['dm_14_plus'] = $yest_dm_plus - ($yest_dm_plus / 14) + $value['plus_dm'];

			$yest_dm_minus = $this->data[$key - 1]['dm_14_minus'];
			$value['dm_14_minus'] = $yest_dm_minus - ($yest_dm_minus / 14) + $value['minus_dm'];

			$yest_tr_sum = $this->data[$key - 1]['tr_14_sum'];
			$value['tr_14_sum'] = $yest_tr_sum - ($yest_tr_sum / 14) + $value['true_range'];

			$value['di_plus'] = $value['dm_14_plus'] / $value['tr_14_sum'];
			$value['di_minus'] = $value['dm_14_minus'] / $value['tr_14_sum'];
			$value['di_difference'] = abs($value['di_plus'] - $value['di_minus']);
			$value['di_sum'] = $value['di_plus'] + $value['di_minus'];
			$value['dx'] = round($value['di_difference'] / $value['di_sum'] * 100);

			if ($key < 27) {
				continue;
			}
			$dx_sum = 0;
			for ($i = ($key - 13); $i <= $key; $i++) {
				$dx_sum += $this->data[$i]['dx'];
			}
			$value['adx'] = round($dx_sum / 14);
			$value['adxr'] = round(($value['adx'] + $this->data[$key - 13]['adx']) / 2);
		}
	}

}