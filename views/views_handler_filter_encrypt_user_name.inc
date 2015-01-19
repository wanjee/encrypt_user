<?php

/**
 * @file
 * Definition of views_handler_filter_user_name.
 */

/**
 * Filter handler for usernames.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_encrypt_user_name extends views_handler_filter_user_name {

  function value_form(&$form, &$form_state) {
    $values = array();
    if ($this->value) {
      $result = db_query("SELECT * FROM {users} u WHERE uid IN (:uids)", array(':uids' => $this->value));
      foreach ($result as $account) {
        if ($account->uid) {
          $name = encrypt_user_get_decrypted_user_field($account, 'name');
          $values[] = $name;
        }
        else {
          $values[] = 'Anonymous'; // Intentionally NOT translated.
        }
      }
    }

    sort($values);
    $default_value = implode(', ', $values);
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Usernames'),
      '#description' => t('Enter a comma separated list of user names.'),
      '#default_value' => $default_value,
      '#autocomplete_path' => 'admin/views/ajax/autocomplete/user',
    );

    if (!empty($form_state['exposed']) && !isset($form_state['input'][$this->options['expose']['identifier']])) {
      $form_state['input'][$this->options['expose']['identifier']] = $default_value;
    }
  }

  /**
   * Validate the user string. Since this can come from either the form
   * or the exposed filter, this is abstracted out a bit so it can
   * handle the multiple input sources.
   */
  function validate_user_strings(&$form, $values) {
    $uids = array();
    $placeholders = array();
    $args = array();
    $results = array();
    foreach ($values as $value) {
      if (strtolower($value) == 'anonymous') {
        $uids[] = 0;
      }
      else {
        $missing[strtolower($value)] = TRUE;
        $args[] = $value;
        $placeholders[] = "'%s'";
      }
    }

    if (!$args) {
      return $uids;
    }

    //$result = db_query("SELECT * FROM {users} WHERE name IN (:names)", array(':names' => $args));
    $result = db_select('users', 'u')
      ->fields('u')
      ->condition('name', $args, 'IN')
      ->addTag('encrypt_user')
      ->execute();
    foreach ($result as $account) {
      $name = encrypt_user_get_decrypted_user_field($account, 'name');
      unset($missing[strtolower($name)]);
      $uids[] = $account->uid;
    }

    if ($missing) {
      form_error($form, format_plural(count($missing), 'Unable to find user: @users', 'Unable to find users: @users', array('@users' => implode(', ', array_keys($missing)))));
    }

    return $uids;
  }

  function admin_summary() {
    // set up $this->value_options for the parent summary
    $this->value_options = array();

    if ($this->value) {
      $result = db_query("SELECT * FROM {users} u WHERE uid IN (:uids)", array(':uids' => $this->value));

      foreach ($result as $account) {
        if ($account->uid) {
          $name = encrypt_user_get_decrypted_user_field($account, 'name');
          $this->value_options[$account->uid] = $name;
        }
        else {
          $this->value_options[$account->uid] = 'Anonymous'; // Intentionally NOT translated.
        }
      }
    }

    return parent::admin_summary();
  }
}
