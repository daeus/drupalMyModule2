<?php
/**
 * @file
 * mymodule2.default_field_validation_rules.inc
 */

/**
 * Implements hook_default_field_validation_rule().
 */
function mymodule2_default_field_validation_rule() {
  $export = array();

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'address';
  $rule->name = 'Address';
  $rule->field_name = 'field_address';
  $rule->col = 'value';
  $rule->entity_type = 'node';
  $rule->bundle = 'supporter';
  $rule->validator = 'field_validation_length_validator';
  $rule->settings = array(
    'min' => '20',
    'max' => '',
    'bypass' => 0,
    'roles' => array(
      1 => 0,
      2 => 0,
      3 => 0,
    ),
    'errors' => 0,
    'condition' => 0,
    'condition_wrapper' => array(
      'condition_field' => '',
      'condition_operator' => 'equals',
      'condition_value' => '',
    ),
  );
  $rule->error_message = 'Please input full address.';
  $export['Address'] = $rule;

  return $export;
}
