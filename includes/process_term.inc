<?php

/**
 * @file
 * Provides a JSON Content import process callback for a taxonomy term.
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
 * @param string $method
 *   The method to use to locate the term.
 * @param array $field_value
 *   The Drupal JSON config for the field.
 *
 * @return string
 *   A term field value from the input config.
 */
function json_content_term_process(
  array $config,
  $method,
  array $field_value) {

  if (empty($field_value) || !module_exists('taxonomy')) {
    return '';
  }

  switch ($method) {
    case 'tid':
      $term = entity_metadata_wrapper('taxonomy_term', $field_value);
      break;

    case 'name':
    default:
      $field_keys = array_keys($field_value);
      $vocab_name = !is_int($field_keys[0]) ? $field_keys[0] : NULL;
      $term_name = isset($field_value[$vocab_name])
        ? $field_value[$vocab_name]
        : $field_value;
      $terms = taxonomy_get_term_by_name($term_name, $vocab_name);
      $term = reset($terms);
      if (!empty($term)) {
        $term = entity_metadata_wrapper('taxonomy_term', $term);
        break;
      }
      if (!$vocab_name) {
        return '';
      }
      $vocab_value = taxonomy_vocabulary_machine_name_load($vocab_name);
      if (!$vocab_value) {
        $vocab_value = new stdClass();
        $vocab_value->name = $vocab_name;
        /** @var \EntityDrupalWrapper $vocabulary */
        $vocabulary = entity_metadata_wrapper('taxonomy_vocabulary',
          $vocab_value);
        $vocabulary->save();
      }
      else {
        $vocabulary = entity_metadata_wrapper('taxonomy_vocabulary',
          $vocab_value);
      }
      $term_value = new stdClass();
      $term_value->name = $term_name;
      $term_value->vid = $vocabulary->getIdentifier();
      /** @var \EntityDrupalWrapper $term */
      $term = entity_metadata_wrapper('taxonomy_term', $term_value);
      $term->save();
      break;
  }
  return $term->getIdentifier();
}
