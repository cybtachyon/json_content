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
 * @param EntityDrupalWrapper $entity
 *   The entity with which to process the paragraph.
 * @param string $name
 *   The name of the paragraphs field.
 * @param string $base_path
 *   The base path for the JSON item file.
 * @param array $field_value
 *   The Drupal JSON config for the field.
 *
 * @return array
 *   A paragraphs field value from the input config.
 */
function json_content_image_process(EntityDrupalWrapper $entity, $name, $base_path, array $field_value) {
  $field = field_info_field($name);
  $columns = array_keys($field['columns']);
  $field_values = $field['cardinality'] === '1'
    ? array($field_value)
    : $field_value;
  $items = array();
  foreach ($field_values as $delta => $value) {
    if (isset($value['path'])) {
      $file_path = $base_path . DIRECTORY_SEPARATOR . $value['path'];
      $file_name = basename($file_path);
      $file_dir = 'public://demo_content';
      file_prepare_directory($file_dir, FILE_CREATE_DIRECTORY);
      $file = file_save_data(file_get_contents($file_path),
        "$file_dir/$file_name");
      if (!isset($file->fid)) {
        continue;
      }
      if ($field['cardinality'] === '1') {
        return array($columns[0] => $file->fid);
      }
      $items[$delta][$columns[0]] = $file->fid;
    }
  }
  return $items;
}
