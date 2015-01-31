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
			// We need at least the adxr.
			if (empty($curr['adxr'])) {
				continue;
			}

			$prev = $data[$key - 1];
			if ($dir = $this->cross($curr, $prev)) {
				// If there is a trade, close it out.
				if ($this->trade) {
					$this->trade->close($curr['close'], $curr);
					$this->trades[] = $this->trade;
					$this->trade = FALSE;
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
		// If we are currently in a trade and the direction we just discerned 
		// matches the direction of the current trade, it means that the lines 
		// simply met, but did not cross. In this case we stay in the current trade.
		if ($this->trade && $this->trade->getDir() === $trade) {
			$trade = FALSE;
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