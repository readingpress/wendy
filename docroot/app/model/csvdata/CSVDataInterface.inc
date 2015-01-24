<?php

/**
 * @file
 * Contains the CSVDataInterface.
 */

/**
 * The CSVData interface.
 */
interface CSVDataInterface 
{

	/**
	 * Loads csv data from a file.
	 *
	 * @param string $filepath
	 *  The path to the file to load.
	 */
	public function __construct($filepath);

	/**
	 * Get the csv data stored in this class instance.
	 *
	 * @return array
	 *  The csv data.
	 */
	public function getCSVData();

}