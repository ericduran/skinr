<?php
// $Id$
/**
 * @file
 * Provide skinr handling for page level rules.
 */

/**
 * @defgroup skinr page rule handlers
 *
 * @{
 */

/**
 * Implementation of hook_skinr_config().
 */
function skinr_skinr_config() {
  $data['rules']['form']['skinr_rule_edit'] = array(
    'index_handler' => 'rules_skinr_form_index_handler',
    'preprocess_hook_callback' => 'rules_skinr_preprocess_hook_callback',
    'title' => t('rule settings'),
    'skinr_weight' => 0,
    'collapsed' => FALSE,
  );
  $data['rules']['form']['skinr_ui_form'] = array(
    'preprocess_hook_callback' => 'rules_skinr_preprocess_hook_callback',
    'title' => t('rule settings'),
    'collapsed' => FALSE,
  );
  $data['rules']['preprocess']['html'] = array(
    'index_handler' => 'rules_skinr_preprocess_index_handler',
  );
  $data['rules']['preprocess']['region'] = array(
    'index_handler' => 'rules_skinr_preprocess_index_handler',
  );
  $data['rules']['contextual_links']['html'] = array(
    'contextual_links_handler' => 'rules_skinr_contextual_links',
  );
  $data['rules']['contextual_links']['region'] = array(
    'contextual_links_handler' => 'rules_skinr_contextual_links',
  );

  return $data;
}

/**
 * Skinr form index handler.
 *
 * @param $op
 *   What kind of action is being performed. Possible values:
 *   - "form": the form elements for Skinr are being inserted in a form
 *   - "submit": the form has been submitted.
 * @param &$form
 *   - For "form", passes in the $form parameter from hook_form_alter().
 *   - For "submit", passes in the $form parameter from hook_form_submit().
 * @param $form_state
 *   - For "form", passes in the $form_state parameter from hook_form_alter().
 *   - For "submit", passes in the $form_state parameter from hook_form_submit().
 * @return
 *   The index where we can find our values in Skinrs data structure.
 */
function rules_skinr_form_index_handler($op, &$form, $form_state) {
  switch ($op) {
    case 'form':
      if (!empty($form['rule']['rid']['#value'])) {
        return $form['rule']['rid']['#value'];
      }
      else {
        return 0;
      }

    case 'submit':
      return $form_state['values']['rid'];
  }
}

/**
 * Skinr preprocess_hook_callback.
 *
 * @param &$form
 *   Passes in the $form parameter from hook_form_alter().
 * @param $form_state
 *   Passes in the $form_state parameter from hook_form_alter().
 * @return
 *   The preprocess_hook we wish to use.
 */
function rules_skinr_preprocess_hook_callback(&$form, $form_state) {
  if (!empty($form['rule'])) {
    return $form['rule']['rule_type']['#value'];
  }
  else {
    $rule = skinr_rule_load($form['skinr']['sid']['#value']);
    return $rule->rule_type;
  }
}

/**
 * Skinr preprocess index handler.
 *
 * @param &$variables
 *   Passes in the $variables parameter from module_preprocess().
 * @return
 *   The index where we can find our values in Skinrs data structure. If an
 *   array is returned, it will loop through each index in Skinr's data
 *   structure and merge the returned classes.
 */
function rules_skinr_preprocess_index_handler(&$variables) {
  if (!empty($variables['region'])) {
    $rule_type = 'region__' . $variables['region'];
  }
  else {
    $rule_type = 'page';
  }
  $rules = skinr_rule_load_multiple(array(), array('rule_type' => $rule_type));

  // Find any page level skinr options and return an array of them.
  $indices = array();
  foreach ($rules as $rule) {
    if (skinr_rule_visible($rule->rid)) {
      $indices[] = $rule->rid;
    }
  }
  return $indices;
}

/**
 * Skinr contextual links handler.
 *
 * @param &$variables
 *   Passes in the $variables parameter from skinr_preprocess().
 * @return
 *   An associative array. Each value is an array that forms the function
 *   arguments for menu_contextual_links(). For example:
 *   @code
 *    $links = array(
 *      'skinr-modulename' => array(
 *        'admin/appearance/skinr/edit', array('system', 'navigation')),
 *      ),
 *      'skinr-modulename-1' => array(
 *        'admin/appearance/skinr/edit', array('system', 'something-else')),
 *      ),
 *    );
 *   @endcode
 */
function rules_skinr_contextual_links(&$variables) {
  if (!empty($variables['region'])) {
    $rule_type = 'region__' . $variables['region'];
  }
  else {
    $rule_type = 'page';
  }
  $rules = skinr_rule_load_multiple(array(), array('rule_type' => 'page'));
  $links = array();
  $counter = 1;

  foreach ($rules as $rule) {
    if (skinr_rule_visible($rule->rid)) {
      $links['skinr-rule-' . $counter++] = array(
        'admin/appearance/skinr/edit', array('page', $rule->rid),
      );
    }
  }
  return $links;
}

/**
 * @}
 */
