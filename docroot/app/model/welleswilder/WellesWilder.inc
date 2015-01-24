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
	 * Adds Welles Wilder's data distinctions to typical market data
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
	}

}