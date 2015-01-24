<?php

/**
 * @file
 * Contains the CSVData class.
 */

/**
 * The CSVData object.
 */
class CSVData implements CSVDataInterface
{

	/**
	 * @var array $csvData
	 *  An array of the data. Each row is an associative array with the 
	 *  appropriate keys.
	 */
	private $csvData = array();

	/**
	 * Load csv data from a file.
	 *
	 * @param string $filepath
	 *  The path to the file to load.
	 */
	public function loadCSV($filepath) {
		$file = fopen($filepath, 'r');
		$keys = fgetcsv($file);
		while (!feof($file)) {
		  $this->csvData[] = array_combine($keys, fgetcsv($file));
		}
		fclose($file);
	}

	/**
	 * Get the csv data stored in this class instance.
	 *
	 * @return array
	 *  The csv data.
	 */
	public function getCSVData() {
		return $this->csvData;
	}

}