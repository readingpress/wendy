<?php

/**
 * @file
 * Contains the DirectionalMovementInterface.
 */

interface DirectionalMovementInterface
{
	
	/**
	 * Get the data.
	 *
	 * @return array
	 *  The data with all calculations performed.
	 */
	public function getData();

	/**
	 * Run the active strategy against the data.
	 */
	public function crunch();

}