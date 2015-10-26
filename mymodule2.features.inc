<?php
/**
 * @file
 * mymodule2.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mymodule2_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_validation" && $api == "default_field_validation_rules") {
    return array("version" => "2");
  }
}

/**
 * Implements hook_views_api().
 */
function mymodule2_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function mymodule2_node_info() {
  $items = array(
    'supporter' => array(
      'name' => t('Supporter'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
