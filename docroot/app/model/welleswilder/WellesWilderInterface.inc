<?php
/**
 * @file
 * Contains the WellesWilderInterface.
 */

interface WellesWilderInterface 
{

	/**
	 * Adds Welles Wilder's data distinctions to typical market data.
	 *
	 * @param array $data
	 *  An array of market data.
	 */
	public function __construct($data);

	/**
	 * Get the trades.
	 *
	 * @return array
	 *  An array of the trades made during testing.
	 */
	public function getTrades();

}