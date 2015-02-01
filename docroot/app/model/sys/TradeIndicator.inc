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

	/**
	 * @var array $indicatorData
	 *  An array of chart data with added indicators.
	 */
	private $indicatorData;

	public function __construct($price_data) {
		$this->priceData = $price_data;
		foreach ($price_data as $key => &$price_bar) {
			$this->getMP($price_bar, $key, 0);
			$this->getMP($price_bar, $key, 2);
			$this->getMP($price_bar, $key, 3);
			$this->getMP($price_bar, $key, 5);
			$this->getMP($price_bar, $key, 8);
			$this->getMP($price_bar, $key, 13);
			$this->getMP($price_bar, $key, 21);
		}
		$this->indicatorData = $price_data;
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
	public function getDataCSV() {
		$file = fopen('tmp/price_data.csv', 'w');
		fputcsv($file, array_keys(reset($this->indicatorData)));
		foreach ($this->indicatorData as $row) {
			fputcsv($file, $row);
		}
		fclose($file);
	}

	private function getMP(&$start_bar, $start_key, $days) {
		if (empty($this->priceData[$start_key - $days])) {
			$start_bar["mp_$days"] = NULL;
			return;
		}
		$segment = array_slice($this->priceData, $start_key - $days, $days + 1);
		$highs = array();
		$lows = array();
		foreach ($segment as $value) {
			$highs[] = $value['high'];
			$lows[] = $value['low'];
		}
		$high = max($highs);
		$low = min($lows);
		$start_bar["mp_$days"] = number_format(($high + $low) / 2, 4);
	}

}