<?php

/**
 * @file
 * Contains the DirectionalMovementSystem class.
 */

class DirectionalMovementSystem extends DirectionalMovement implements DirectionalMovementInterface
{

	/**
	 * @var mixed $trade
	 *  The current trade (an object), or FALSE if there isn't one.
	 */
	private $trade = FALSE;

	/**
	 * @var array $trades
	 *  An array of all completed trades.
	 */
	private $trades = array();

	/**
	 * Run the active strategy against the data.
	 */
	public function crunch() {
		$data = $this->getData();
		foreach ($this->getData() as $key => $curr) {

			// We need at least a current and a previous bar
			if ($key === 0) {
				continue;
			}

			/*if ($this->trade && $tp = $this->takeProfit($curr, $prev)) {
				$this->trade->close($tp, $curr);
				$this->trades[] = $this->trade;
				$this->trade = FALSE;
			}

			if ($this->trade && $stop = $this->trade->stop($curr)) {
				$this->trade->close($stop, $curr);
				$this->trades[] = $this->trade;
				$this->trade = FALSE;
			}*/

			$prev = $data[$key - 1];
			if (/*!$this->trade &&*/ $dir = $this->cross($curr, $prev)) {
				// @TODO remove this maybe. The first condition above can be removed to
				// see the earlier state of the system.
				if ($this->trade) {
					$this->trade->close($curr['close'], $curr);
					$this->trades[] = $this->trade;
				}
				$this->trade = new DirectionalMovementTrade($curr['close'], $dir, $curr);
			}
		}

	}

	/**
	 * Get the trades.
	 */
	public function getTrades() {
		return $this->trades;
	}

	/**
	 * Checks to see whether the positive direction index and the negative 
	 * directional index have crossed.
	 *
	 * @param array $curr
	 *	The current time bar.
	 * @param array $prev
	 *  The previous time bar.
	 */
	private function cross($curr, $prev) {
		if (!isset($prev['adx'])) {
			return FALSE;
		}
		$trade = FALSE;

		$prev_di = array_intersect_key($prev, array('plus_di' => '', 'minus_di' => ''));
		$prev_king = array_keys($prev_di, max($prev_di));
		$curr_di = array_intersect_key($curr, array('plus_di' => '', 'minus_di' => ''));
		$curr_king = array_keys($curr_di, max($curr_di));

		if ($curr['plus_di'] != $curr['minus_di'] && $prev_king != $curr_king) {
			switch (reset($curr_king)) {
				case 'plus_di':
					$trade = 'long';
					break;
				case 'minus_di':
					$trade = 'short';
					break;
			}
		}
		return $trade;
	}

	/**
	 * Return whether a current trade should settle out based on the take profit
	 * rules.
	 *
	 * @param array $curr
	 *  The current bar.
	 * @param array $prev
	 *  The previous bar.
	 *
	 * @return mixed
	 *  The take profit if it has been hit, or false if not.
	 */
	private function takeProfit($curr, $prev) {
		$adx_on_top = $curr['adx'] > $curr['plus_di'] && $curr['adx'] > $curr['minus_di'];
		if ($adx_on_top && $curr['adx'] < $prev['adx']) {
			return $curr['close'];
		}
		return FALSE;
	}

}