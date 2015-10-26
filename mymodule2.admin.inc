<?php

/**
 * @file
 * Configuration page for mymodule2.
 */

/**
 * Form builder.
 *
 * This function checks if the required nodesn  exist, or create new ones. The
 * retrieve the node form and combine them as one.
 */
function mymodule2_settings_form() {
  global $user;
  $form = array();
  $nodes = _mymodule2_get_nodes();
  $languages = language_list();

  foreach ($languages as $language) {
    if ($language->enabled) {
      $langcode = $language->language;

      $form[$langcode] = array(
        '#title' => t('Contact person') . ' (' . $language->name . ')',
        '#type' => 'fieldset',
        '#tree' => TRUE
      );
      $form[$langcode]['language'] = array(
        '#type' => 'value',
        '#value' => $langcode,
      );
      $form[$langcode]['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#required' => TRUE
      );
      $form[$langcode]['email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#required' => TRUE,
      );
      $form[$langcode]['address'] = array(
        '#type' => 'textfield',
        '#title' => t('Address'),
        '#required' => TRUE,
      );
      $form[$langcode]['country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => countries_get_countries('name', array('enabled' => COUNTRIES_ENABLED)),
        '#required' => TRUE,
      );

      if (isset($nodes[$langcode])) {
        $node = $nodes[$langcode];
        $wrapper = entity_metadata_wrapper('node', $node);

        $form[$langcode]['name']['#default_value'] = $wrapper->title->value();
        $form[$langcode]['email']['#default_value'] = $wrapper->field_email->value();
        $form[$langcode]['address']['#default_value'] = $wrapper->field_address->value();
        $form[$langcode]['country']['#default_value'] = $wrapper->field_country->value()->iso2;
      }
    }
  }

  // copy from system_settings_form();
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  return $form;
}

/**
 * Submit handler.
 */
function mymodule2_settings_form_submit(&$form, &$form_state) {
  global $user;

  $values = $form_state['values'];
  $nodes = _mymodule2_get_nodes();
  $languages = language_list();

  foreach ($languages as $language) {
    if ($language->enabled) {
      $langcode = $language->language;
      $value = $values[$langcode];

      if (isset($nodes[$langcode])) {
        $node = $nodes[$langcode];
      } else {
        $node = entity_create('node', array(
          'type' => MYMODULE2_TYPE,
          'uid' => $user->uid,
          'status' => NODE_PUBLISHED,
          'language' => $value['language'],
        ));
      }
      $wrapper = entity_metadata_wrapper('node', $node);
      $wrapper->title->set($value['name']);
      $wrapper->field_email->set($value['email']);
      $wrapper->field_address->set($value['address']);
      $wrapper->field_country->set(country_load($value['country']));
      $wrapper->save();
    }
  }
}

/**
 * Validate handler for email.
 */
function _mymodule_email_validate($element, &$form_state, $form) {
  if (!filter_var($element['#value'], FILTER_VALIDATE_EMAIL)) {
    form_error($element, t('Please input valid email.'));
  }
}

/**
 * Validate handler for address.
 */
function _mymodule_address_validate($element, &$form_state, $form) {
  if (strlen(trim($element['#value'])) < 21) {
    form_error($element, t('Please input full address.'));
  }
}

/**
 * Get supporters node.
 */
function _mymodule2_get_nodes() {
  $langcode = array();
  $languages = language_list();
  foreach ($languages as $language) {
    if ($language->enabled) {
      $langcodes[] = $language->language;
    }
  }

  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', MYMODULE2_TYPE)
    ->propertyCondition('status', NODE_PUBLISHED)
    ->propertyCondition('language', $langcodes, 'IN')
    ->addMetaData('account', user_load(1));
  $result = $query->execute();

  $nodes = array();
  foreach ($result['node'] as $data) {
    $nid = $data->nid;
    $node = node_load($nid);
    $nodes[$node->language] = $node;
  }

  return $nodes;
}
