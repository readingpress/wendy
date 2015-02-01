<?php

/**
 * @file
 * Bootstrap the application.
 */

require_once WENDY_ROOT . '/app/model/csvdata/CSVDataInterface.inc';
require_once WENDY_ROOT . '/app/model/csvdata/CSVData.inc';
require_once WENDY_ROOT . '/app/model/trade/TradeInterface.inc';
require_once WENDY_ROOT . '/app/model/trade/Trade.inc';
require_once WENDY_ROOT . '/app/model/directionalmovement/DirectionalMovementTrade.inc';
require_once WENDY_ROOT . '/app/model/directionalmovement/DirectionalMovementInterface.inc';
require_once WENDY_ROOT . '/app/model/directionalmovement/DirectionalMovement.inc';
require_once WENDY_ROOT . '/app/model/directionalmovement/DirectionalMovementSystem.inc';
require_once WENDY_ROOT . '/app/model/directionalmovement/TrendBalancePointSystem.inc';
require_once WENDY_ROOT . '/app/model/directionalmovement/TrendBalancePointTrade.inc';
require_once WENDY_ROOT . '/app/model/sys/TradeIndicator.inc';