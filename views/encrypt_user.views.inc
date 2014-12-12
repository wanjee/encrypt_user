<?php
/**
 * @file
 * Functions for compatibility between dbee module and Views module
 */

/**
 * Implements hook_views_data_alter().
 */
function encrypt_user_views_data_alter(&$data) {
  foreach ($data as $table => $config) {
    foreach ($config as $item => $item_config) {
      if (isset($item_config['field']['handler']) && $item_config['field']['handler'] == 'views_handler_field_user_name') {
        $data[$table][$item]['field']['handler'] = 'views_handler_field_encrypt_user_user_name';
      }

      if (isset($item_config['field']['handler']) && $item_config['field']['handler'] == 'views_handler_field_user_mail') {
        $data[$table][$item]['field']['handler'] = 'views_handler_field_encrypt_user_user_mail';
      }
    }
  }
}
