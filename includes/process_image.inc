<?php

/**
 * @file
 * Provides a JSON Content import process callback for a reference.
 *
 * @see https://www.drupal.org/node/2621866
 */

/**
 * Reference process callback for json_content_CALLBACK_process.
 *
 * @param array $config
 *   The current JSON Content import configuration.
 *   - EntityDrupalWrapper $entity
 *     The entity with which to process the paragraph.
 *   - string $name
 *     The name of the paragraphs field.
 *   - string $base_path
 *     The base path for the JSON item file.
 * @param array $field_value
 *   The Drupal JSON config for the image field value.
 *
 * @return array
 *   An image file value from the input config.
 */
function json_content_image_process(array $config, array $field_value) {
  /** @var string $base_path */
  list(, , $base_path) = $config;
  $value = array();
  if (!isset($field_value['path'])) {
    return $value;
  }
  $file_path = $base_path . DIRECTORY_SEPARATOR . $field_value['path'];
  $file_name = basename($file_path);
  $file_dir = 'public://demo_content';
  file_prepare_directory($file_dir, FILE_CREATE_DIRECTORY);
  if (!file_exists($file_path)) {
    return $value;
  }
  $file = file_save_data(file_get_contents($file_path),
    $file_dir . DIRECTORY_SEPARATOR . $file_name);
  if (empty($file->fid)) {
    return $value;
  }
  $value['fid'] = $file->fid;
  return $value;
}
