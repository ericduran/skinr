<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the skinr module.
 */

/**
 * Menu callback; displays the skinr administration page.
 *
 * @todo Placeholder settings page.
 */
function skinr_admin() {
  if (module_exists('devel')) {
    $themes = list_themes();
    ksort($themes);

    foreach ($themes as $theme) {
      if (!$theme->status) {
        continue;
      }

      drupal_set_message(t('Theme %theme', array('%theme' => $theme->info['name'])));
      dpm(skinr_get($theme->name));
    }
  }
  $output = '';
  return $output;
}
