<?php
/**
 * @file
 * Field handler to provide acess control for the encrypted email field
 */

/**
 * A handler to display the uncrypted email version.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_encrypt_user_user_mail extends views_handler_field_user_mail {

  /**
   * Add data fields to query fields
   */
  function query() {
    // add this field to make it available in render
    $tableAlias = $this->getUserTableAlias($this->query->table_queue);
    $this->query->add_field($tableAlias, 'data', 'users_data_encrypt');
    // In all cases, go through the parent default way
    return parent::query();
  }

  function render($values) {
    if ($values) {
      if (is_string($values->users_data_encrypt)) {
        $values->users_data_encrypt = unserialize($values->users_data_encrypt);
      }

      $field_name = $this->table . '_mail';
      $values->{$field_name} = encrypt_user_decrypt_data($values->users_data_encrypt['encrypt_mail']);
    }

    // In all cases, go through the parent default way
    return parent::render($values);
  }

  private function getUserTableAlias($table_queue) {
    foreach ($table_queue as $tableDef) {
      if ($tableDef['table'] == 'users') {
        return $tableDef['alias'];
      }
    }
    //still here? we return a reasonable default
    return 'users';
  }
}
