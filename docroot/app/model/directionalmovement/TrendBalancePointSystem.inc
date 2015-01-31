<?php

/**
 * @file
 * Contains the TrendBalancePointSystem class.
 */

class TrendBalancePointSystem extends DirectionalMovement implements DirectionalMovementInterface
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
		foreach ($data as $key => $curr) {
			if (!isset($curr['adxr'])) {
				continue;
			}

			if ($this->trade && $stop = $this->trade->stop($curr)) {
				$this->trade->close($stop, $curr);
				$this->trades[] = $this->trade;
				$this->trade = FALSE;
			}

			if ($this->trade && $tp = $this->trade->takeProfit($curr)) {
				$this->trade->close($tp, $curr);
				$this->trades[] = $this->trade;
				$this->trade = FALSE;				
			}

			if ($this->trade) {
				$this->trade->setStop($curr);
				$this->trade->setTakeProfit($curr);				
			}

			$prev_1 = $data[$key - 1];
			$prev_2 = $data[$key - 2];
			if ($dir = $this->trade($curr, $prev_1, $prev_2)) {
				// If there is a trade and it is in a different direction, close it out.
				if ($this->trade && $this->trade->getDir() != $dir) {
					$this->trade->close($curr['close'], $curr);
					$this->trades[] = $this->trade;
					$this->trade = FALSE;
				}
				$this->trade = new TrendBalancePointTrade($curr['close'], $dir, $curr);	
				$this->trade->setStop($curr);
				$this->trade->setTakeProfit($curr);
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
	 * Get the trade for the current bar.
	 *
	 * @param array $curr
	 *  The current bar.
	 * @param array $prev_1
	 *  The previous bar
	 * @param array $prev_2
	 *  Two bars ago.
	 */
	private function trade($curr, $prev_1, $prev_2) {
		$trade = FALSE;
		if ($curr['mf'] > $prev_1['mf'] && $curr['mf'] > $prev_2['mf']) {
			$trade = 'long';
		}
		if ($curr['mf'] < $prev_1['mf'] && $curr['mf'] < $prev_2['mf']) {
			$trade = 'short';
		}
		return $trade;
	}

}