<?php

/**
 * @file
 * Provides a JSON Content import process callback for a paragraph.
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
function json_content_paragraph_process(
  EntityDrupalWrapper $entity,
  $name,
  $base_path,
  array $field_value) {

  $entity_value = $entity->value();
  if (empty($field_value)) {
    return NULL;
  }
  /** @var \ParagraphsItemEntity $pg_value */
  $pg_value = entity_create('paragraphs_item', array(
    'bundle' => $field_value['type'],
    'field_name' => $name,
  ));
  unset($field_value['type']);
  $pg_value->setHostEntity($entity->type(), $entity->value());
  /** @var \EntityDrupalWrapper $paragraph */
  $paragraph = entity_metadata_wrapper('paragraphs_item', $pg_value);
  $paragraph = json_content_import_entity_properties($paragraph,
    $field_value,
    $base_path);
  try {
    $paragraph->value()->save(TRUE);
  }
  catch (Exception $e) {
    watchdog('error', $e->getMessage());
    return NULL;
  }
  drupal_static_reset('field_language');
  // There is an issue with setting revision ID via EMD.
  // @see https://www.drupal.org/node/2621866
  $entity->set($entity_value);
  return $paragraph->value();
}
