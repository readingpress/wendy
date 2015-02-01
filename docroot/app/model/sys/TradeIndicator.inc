<?php

/**
 * @file
 * Contains the TradeIndicator class.
 */

/**
 * The TradeIndicator class. Provides various indicators on top of price.
 *
 * @param array $price_data
 *  An array of chart data.
 */
class TradeIndicator
{

	/**
	 * @var array $priceData
	 *  An array of chart data.
	 */
	private $priceData;

	public function __construct($price_data) {
		$this->priceData = $price_data;
	}

	/**
	 * Get the unadulterated price data.
	 *
	 * @return array
	 *  The chart data that was passed into the constructor.
	 */
	public function getPriceData() {
		return $this->priceData;
	}

	/**
	 * Get the unadulterated price data.
	 *
	 * @return array
	 *  The chart data that was passed into the constructor.
	 */
	public function getPriceDataCSV() {
		$file = fopen('tmp/price_data.csv', 'w');
		fputcsv($file, array_keys(reset($this->priceData)));
		foreach ($this->priceData as $row) {
			fputcsv($file, $row);
		}
		fclose($file);
	}

}