<?php

/**
 * @file
 * Contains the ParabolicTimePriceSystem class.
 */

class ParabolicTimePriceSystem extends WellesWilder implements WellesWilderInterface
{

	/**
	 * @var integer $sar
	 *  The stop and reversal price.
	 */
	private $sar;

	/**
	 * @var integer $af
	 *  The acceleration factor.
	 */
	private $af;

	/**
	 * Adds Welles Wilder's Parabolic Time/Price System data distinctions to 
	 * typical market data.
	 *
	 * @param array $data
	 *  An array of market data.
	 */
	public function __construct($data) {
		parent::__construct($data);
		$this->af = .02;
		$this->setFirst();
	}

	/**
	 * Find the first trade.
	 */
	protected function setFirst() {
		// For this strategy, to find the first trade, we iterate over the data,
		// finding the first high point (hip) or low point (lop), whichever comes
		// first. If a high point comes first, we enter short. If a low point comes
		// first, we enter long.
		foreach ($this->data as $key => $bar) {
			if ($bar['hip']) {
				$this->currentTrade = new Trade('short', $bar['close'], $bar);
				$this->current = $key;
				$this->sar = $bar['high'];
				break;
			}

			if ($bar['lop']) {
				$this->currentTrade = new Trade('long', $bar['close'], $bar);
				$this->current = $key;
				$this->sar = $bar['low'];
				break;
			}

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
	 * Crunches an interval for a short trade.
	 *
	 * @param array $curr
	 *  The current bar.
	 * @param array $prev
	 *  The previous bar.
	 */
	private function shortInterval($curr, $prev) {
		// Check to see if there is a new low. If there is, we need to increase the
		// af (acceleration factor) by .02.
		$new_low = $curr['low'] < $this->currentTrade->getLow();
		$this->currentTrade->crunchBar($curr);
		// The trade is over. We need to reverse.
		if ($curr['high'] > $this->sar) {
			$this->currentTrade->close($this->sar, $curr);
			$this->trades[] = $this->currentTrade;
			$sip = $this->currentTrade->getLow();
			$this->currentTrade = new Trade('long', $this->sar, $curr);
			$this->af = .02;
			$this->sar = $sip;
			return;
		}
		// Next, update the sar. First we obtain the difference between the lowest
		// price reached while in the trade and the current sar.
		$diff = $this->sar - $this->currentTrade->getLow();
		// Next, we multiply the difference by the current af (acceleration factor).
		$diff *= $this->af;
		// Finally, we subtract the result to the sar for this bar to get the sar 
		// for the next bar.
		$this->sar -= $diff;
		// We never put the new sar below the previous days high, or todays high. If
		// it calculates out to be below either, we instead set it to the higher of 
		// the two (previous high and today's high).
		if ($this->sar < $curr['high'] || $this->sar < $prev['high']) {
			$this->sar = $curr['high'] > $prev['high'] ? $curr['high'] : $prev['high'];
		}
		// If this bar has a new low, we need to increase the af (acceleration 
		// factor by .02 up to a maximum of 0.2).
		if ($this->af < 0.2 && $new_low) {
			$this->af += .02;
		}
	}

	/**
	 * Crunches an interval for a long trade.
	 *
	 * @param array $curr
	 *  The current bar.
	 * @param array $prev
	 *  The previous bar.
	 */
	private function longInterval($curr, $prev) {
		// Check to see if there is a new high. If there is, we need to increase the
		// af (acceleration factor) by .02.
		$new_high = $curr['high'] > $this->currentTrade->getHigh();
		$this->currentTrade->crunchBar($curr);
		// The trade is over. We need to reverse.
		if ($curr['low'] < $this->sar) {
			$this->currentTrade->close($this->sar, $curr);
			$this->trades[] = $this->currentTrade;
			$sip = $this->currentTrade->getHigh();
			$this->currentTrade = new Trade('short', $this->sar, $curr);
			$this->af = .02;
			$this->sar = $sip;
			return;
		}
		// Next, update the sar. First we obtain the difference between the highest
		// price reached while in the trade and the current sar.
		$diff = $this->currentTrade->getHigh() - $this->sar;
		// Next, we multiply the difference by the current af (acceleration factor).
		$diff *= $this->af;
		// Finally, we add the result to the sar for this bar to get the sar for the
		// next bar.
		$this->sar += $diff;
		// We never put the new sar above the previous days low, or todays low. If
		// it calculates out to be above either, we instead set it to the lower of 
		// the two (previous low and today's low).
		if ($this->sar > $curr['low'] || $this->sar > $prev['low']) {
			$this->sar = $curr['low'] < $prev['low'] ? $curr['low'] : $prev['low'];
		}
		// If this bar has a new high, we need to increase the af (acceleration 
		// factor by .02 up to a maximum of 0.2).
		if ($this->af < 0.2 && $new_high) {
			$this->af += .02;
		}
	}

}