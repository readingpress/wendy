<?php

/**
 * @file
 * Contains the DirectionalMovement class.
 */

class DirectionalMovement implements DirectionalMovementInterface
{

	/**
	 * @var integer $average_period
	 *  The number of intervals to use in averages for the data in this system.
	 */
	private $average_period = 14;

	/**
	 * @var array $data
	 *  And array of chart bars
	 */
	private $data = array();

	/**
	 * Add Directional Movement Data to Market Information.
	 *
	 * @param array $data
	 *  An array of market data.
	 */
	public function __construct($data) {

		foreach ($data as $key => &$curr) {

			if ($key === 0) {
				continue;
			}

			$prev =  $data[$key - 1];
			$curr['tr_1'] = $this->trueRange($curr, $prev);
			$curr = array_merge($curr, $this->directionalMovement($curr, $prev));
			// We continue until we have enough bars to apply our average period.
			if ($key < $this->average_period) {
				continue;
			}
			// Once we have reached enough bars in order to apply our average period
			// we'll create sums of total range and both positive and negative
			// directional movement.
			if ($key === $this->average_period) {
				// We only ever calculate the sum of the last x days this way once.
				$curr = array_merge($curr, $this->sums($data));
				continue;
			}
			// In each subsequent iteration, we use an accumulation technique. This
			// has the advantage of applying a smoothing effect to the data.
			$this->proxySums($curr, $prev);
			// We now have enough data to add our directional indicators
			$this->directionalIndicators($curr);
			if ($key < (2 * $this->average_period)) {
				continue;
			}
			// Once we have reached double our average period, we can add our first
			// average directional index.
			if ($key === (2 * $this->average_period)) {
				// Slice the array on the average period, by the average period. We add
				// one to the offset because we skipped the first item.
				$slice = array_slice($data, $this->average_period + 1, $this->average_period);
				$curr = array_merge($curr, $this->adx($slice));
				continue;
			}
			// Once we have calculated the adx once, from there on out we use a moving
			// average calculation method rather than a traditional average.
			$this->proxyADX($curr, $prev);
			// Once we have adx values for an interval of time that is equivalent to
			// the average period, we can finally calculate the Average Directional
			// index rating.
			if (isset($data[$key - $this->average_period + 1]['adx'])) {
				$this->adxr($curr, $data[$key - $this->average_period + 1]);
			}
		}
		$this->data = $data;
	}

	/**
	 * Get the data.
	 *
	 * @return array
	 *  The data with all calculations performed.
	 */
	public function getData() {
		return $this->data;
	}

	/**
	 * Run the active strategy against the data. It is empty in this class, 
	 * because each child class will make its own implementation.
	 */
	public function crunch() {

	}

	/**
	 * Get the true range for a time bar.
	 *
	 * @param array $curr
	 *	The current time bar.
	 * @param array $prev
	 *  The previous time bar.
	 *
	 * @return float
	 *  The true range for the time bar.
	 */
	private function trueRange($curr, $prev) {
		// The distance from the current bar's high to the previous bar's low.
		$a = $curr['high'] - $curr['low'];
		// The distance from previous bar's close to the current bar's high.
		$b = $curr['high'] - $prev['close'];
		// The distance from previous bar's close to current bar's low.
		$c = $prev['close'] - $curr['low'];
		return max(array($a, $b, $c));
	}

	/**
	 * Get the directional movement for a time bar.
	 *
	 * @param array $curr
	 *	The current time bar.
	 * @param array $prev
	 *  The previous time bar.
	 *
	 * @return array
	 *  An array containing both positive and negative directional movement for
	 *  the bar.
	 */
	private function directionalMovement($curr, $prev) {
		$dm = array(
			'plus_dm_1' => 0,
			'minus_dm_1' => 0,
		);
		// Positive movement is the current bar's high minus the previous bar's 
		// high.
		$plus_dm = $curr['high'] - $prev['high'];
		// Negative movement is the previous bars low minus the current bars low.
		$minus_dm = $prev['low'] - $curr['low'];
		// If both positive movement and negative movement are less than or equal to
		// zero, there was no directional movement in this bar.
		if ($plus_dm <= 0 && $minus_dm <= 0) {
			return $dm;
		}
		// At this point we know that at least one of our movement variables has a
		// positive value. We choose the greatest one as the indicator of movement
		// for the bar.
		if ($plus_dm > $minus_dm) {
			$dm['plus_dm_1'] = $plus_dm;
		}
		else {
			$dm['minus_dm_1'] = $minus_dm;
		}
		return $dm;
	}

	/**
	 * Get the sums of total range, and positive and well as negative directional
	 * movement for a period of time matching the average period.
	 *
	 * @param array $data
	 *  An array of market data.
	 */
	private function sums($data) {
		$tr_sum = $plus_dm_sum = $minus_dm_sum = 0;
		for ($i = 1; $i <= $this->average_period; $i++) {
			$tr_sum += $data[$i]['tr_1'];
			$plus_dm_sum += $data[$i]['plus_dm_1'];
			$minus_dm_sum += $data[$i]['minus_dm_1'];
		}
		return array(
			"tr_$this->average_period" => $tr_sum,
			"atr_$this->average_period" => $tr_sum / $this->average_period,
			"plus_dm_$this->average_period" => $plus_dm_sum,
			"minus_dm_$this->average_period" => $minus_dm_sum,
		);
	}

	/**
	 * Get the sums of total range, and positive and well as negative directional
	 * movement for a period of time matching the average period using an 
	 * approximation technique rather than actual addition. This smoothes out the
	 * end data.
	 *
	 * @param array $curr
	 *	The current time bar. Passed by reference.
	 * @param array $prev
	 *  The previous time bar.
	 */
	private function proxySums(&$curr, $prev) {
		foreach (array('tr', 'plus_dm', 'minus_dm') as $key) {
			$prev_value = $prev["{$key}_{$this->average_period}"];
			// We take the previous bar's value for this sum, and then subtract a chunk
			// that is exactly one average period's worth.
			$chunk = $prev_value / $this->average_period;
			// We then subtract that chunk from the total previous value and then add
			// in the current bar's single value in its place.
			$curr["{$key}_{$this->average_period}"] = $prev_value - $chunk + $curr["{$key}_1"];
		}
		$curr["atr_{$this->average_period}"] = $curr["tr_{$this->average_period}"] / $this->average_period;
	}

	/**
	 * Add the positive and negative directional indicators to the current bar's
	 * data. Also adds he difference and sum of the positive and negative 
	 * directional indicators.
	 *
	 * @param array $curr
	 *	The current time bar. Passed by reference.
	 */
	private function directionalIndicators(&$curr) {
		foreach (array('plus', 'minus') as $key) {
			// This calculation is a simple percentage of positive or negative 
			// movement over the true range sum for the average period.
			$movement = $curr["{$key}_dm_{$this->average_period}"];
			$range = $curr["tr_{$this->average_period}"];
			$curr["{$key}_di"] = round($movement / $range * 100);
		}
		$curr['di_diff'] = abs($curr['plus_di'] - $curr['minus_di']);
		$curr['di_sum'] = $curr['plus_di'] + $curr['minus_di'];
		// Our last calculation is the directional movement index. This is obtained
		// by dividing the difference between the positive and negative di 
		// indicators by their sum, creating a number that always falls between 0 
		// and 100
		$curr['dx'] = round($curr['di_diff'] / $curr['di_sum'] * 100);
	}

	/**
	 * Calculate the ADX from a slice. It is just an average of all the DX values 
	 * in the slice.
	 *
	 * @param array $slice
	 *  A slice of market data to take the adx from.
	 *
	 * @return array
	 *  An array with the adx value, suitable to merge into a market day using
	 *  array_merge.
	 */
	private function adx($slice) {
		$dx_sum = 0;
		foreach ($slice as $key => $value) {
			$dx_sum += $value['dx'];
		}
		return array('adx' => round($dx_sum / count($slice)));
	}

	/**
	 * Add the ADX from the current and previous bars using a moving average 
	 * technique 
	 *
	 * @param array $curr
	 *	The current time bar. Passed by reference.
	 * @param array $prev
	 *  The previous time bar.
	 */
	private function proxyADX(&$curr, $prev) {
		// First we multiply the previous bars ADX by one minus the average period.
		$calc = $prev['adx'] * ($this->average_period - 1);
		// Then we add the current DX, and divide the whole thing by the average
		// period.
		$curr['adx'] = round(($calc + $curr['dx']) / $this->average_period);
	}

	/**
	 * Add the Average Directional Index Rating to the current bars data.
	 *
	 * @param array $curr
	 *	The current time bar. Passed by reference.
	 * @param array $prev
	 *  The time bar x days ago, where x is the average period.
	 */
	private function adxr(&$curr, $prev) {
		$curr['adxr'] = round(($curr['adx'] + $prev['adx']) / 2);
	}

}