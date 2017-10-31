<?php

/**
 * @file
 * Drush functions for the json_content module.
 */

use Drush\Log\LogLevel;

/**
 * Implements hook_drush_command().
 */
function json_content_drush_command() {
  $items = array();
  $items['json-content-import'] = array(
    'description' => 'Import a JSON file in Drupal format as content.',
    'aliases' => array('jci'),
    'arguments' => array(
      'file' => 'The path to the JSON file that will be imported.',
    ),
    'examples' => array(
      'json-content-import ./files/json/example_content.json' =>
      'Import an example JSON file as content.',
    ),
  );
  return $items;
}

/**
 * Imports a JSON file in Drupal format as content.
 *
 * @param string $file
 *   The path to the JSON file that will be imported.
 */
function drush_json_content_import($file) {
  $base_path = dirname($file);
  $json_file = basename($file);
  $result = json_content_import($base_path, $json_file);
  if (is_array($result)) {
    /** @var array[] $ci */
    $ci += $result;
  }
  if (!empty($ci['errors'])) {
    /** @var \Exception $error */
    foreach ($ci['errors'] as $error) {
      drush_log($error->getMessage(), LogLevel::WARNING);
    }
    drush_log(dt('Error importing JSON Content from @file. See log for info.',
      array('@module' => $file)), LogLevel::ERROR);
    return;
  }
  $count = count($ci);
  $message = format_plural(
    $count,
    'Successfully imported one content type from @file.',
    'Successfully imported @count content types from @file.',
    array('@file' => $file));
  drush_log($message, LogLevel::OK);
}
