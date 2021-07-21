<?php

/**
 * Page callback
 */
function manager_adding_bonuses_page() {
  drupal_add_js(
    drupal_get_path(
      'module',
      'manager_bonus_account'
    ) . '/js/manager_adding_bonuses_page.js'
  );
  drupal_add_css(
    drupal_get_path(
      'module',
      'manager_bonus_account'
    ) . '/css/manager_adding_bonuses_page.css'
  );

  $output = '';

  $output .= '<div id="manager-adding-bonuses-form-wrapper">';

  $manager_adding_bonuses_form = drupal_get_form('manager_adding_bonuses_form');
  $output .= drupal_render($manager_adding_bonuses_form);

  $output .= '</div>';

  $output .= '<h2 id="history-bonus-header">История зачисления бонусов</h2>';

  $user_bonus_history_filter_form = drupal_get_form(
    'user_bonus_history_filter_form'
  );
  $output .= drupal_render($user_bonus_history_filter_form);

  if (empty($_GET['filter-user-name'])) {
    $output .= get_history_all();
  }
  else {
    $output .= get_history_user(trim($_GET['filter-user-name']));
  }

  return $output;
}

function get_history_all() {
  $output = '';

  $query = db_select('commerce_product');
  $query->fields('commerce_product', ['product_id']);
  $query_result = $query->execute()->fetchAll();

  $products_ids = [];
  foreach ($query_result as $query_result_value) {
    array_push($products_ids, $query_result_value->product_id);
  }

  $current_user_bonus_products = commerce_product_load_multiple($products_ids);

  $bonus_history_data = [];
  foreach ($current_user_bonus_products as $current_user_bonus_products_value) {
    foreach ($current_user_bonus_products_value->field_bonus_history['und'] as $bonus_history_value) {
      $bonus_history_value_string = trim($bonus_history_value['value']);

      if (!empty($bonus_history_value_string) && $bonus_history_value_string != '0') {
        $bonus_history_value_array = explode('-', $bonus_history_value_string);

        if (count($bonus_history_value_array) == 2) {
          array_push(
            $bonus_history_value_array,
            $current_user_bonus_products_value->sku
          );
          array_push($bonus_history_data, $bonus_history_value_array);
        }
      }
    }
  }

  //Сортировка (Пузырьком) -
  $bonus_history_data_count = count($bonus_history_data);
  for ($i = 0; $i < $bonus_history_data_count - 1; $i++) {
    for ($j = $i + 1; $j < $bonus_history_data_count; $j++) {
      if ($bonus_history_data[$i][0] < $bonus_history_data[$j][0]) {
        $temp = $bonus_history_data[$i];
        $bonus_history_data[$i] = $bonus_history_data[$j];
        $bonus_history_data[$j] = $temp;
      }
    }
  }
  // - Сортировка (Пузырьком)

  $output .= '<div id="history-bonus-wrapper">';
  $output .= '<table id="history-bonus-table">';
  $output .= '<tr class="table-header">';
  $output .= '<th>';
  $output .= 'Имя пользователя';
  $output .= '</th>';
  $output .= '<th>';
  $output .= 'Дата зачисления';
  $output .= '</th>';
  $output .= '<th>';
  $output .= 'Сумма';
  $output .= '</th>';
  $output .= '</tr>';
  foreach ($bonus_history_data as $bonus_history_data_key => $bonus_history_data_value) {
    $output .= '<tr class="row-' . $bonus_history_data_key . ' row-' . ($bonus_history_data_key % 2 == 0 ? 'even' : 'odd') . '">';
    $output .= '<td>';
    $output .= $bonus_history_data_value[2];
    $output .= '</td>';
    $output .= '<td>';
    $output .= date('d.m.Y / H:i:s', $bonus_history_data_value[0]);
    $output .= '</td>';
    $output .= '<td>';
    $output .= $bonus_history_data_value[1];
    $output .= '</td>';
    $output .= '</tr>';
  }
  $output .= '</table>';
  $output .= '</div>';

  return $output;
}

function get_history_user($user_name) {
  $output = '';

  $current_user_bonus_product = commerce_product_load_by_sku($user_name);

  if ($current_user_bonus_product !== FALSE) {
    $bonus_history_data = [];
    foreach ($current_user_bonus_product->field_bonus_history['und'] as $bonus_history_value) {
      $bonus_history_value_string = trim($bonus_history_value['value']);

      if (!empty($bonus_history_value_string) && $bonus_history_value_string != '0') {
        $bonus_history_value_array = explode('-', $bonus_history_value_string);

        if (count($bonus_history_value_array) == 2) {
          array_push($bonus_history_data, $bonus_history_value_array);
        }
      }
    }

    //Сортировка (Пузырьком) -
    $bonus_history_data_count = count($bonus_history_data);
    for ($i = 0; $i < $bonus_history_data_count - 1; $i++) {
      for ($j = $i + 1; $j < $bonus_history_data_count; $j++) {
        if ($bonus_history_data[$i][0] < $bonus_history_data[$j][0]) {
          $temp = $bonus_history_data[$i];
          $bonus_history_data[$i] = $bonus_history_data[$j];
          $bonus_history_data[$j] = $temp;
        }
      }
    }
    // - Сортировка (Пузырьком)

    $output .= '<div id="history-bonus-wrapper">';
    $output .= '<table id="history-bonus-table">';
    $output .= '<tr class="table-header">';
    $output .= '<th>';
    $output .= 'Имя пользователя';
    $output .= '</th>';
    $output .= '<th>';
    $output .= 'Дата зачисления';
    $output .= '</th>';
    $output .= '<th>';
    $output .= 'Сумма';
    $output .= '</th>';
    $output .= '</tr>';
    foreach ($bonus_history_data as $bonus_history_data_key => $bonus_history_data_value) {
      $output .= '<tr class="row-' . $bonus_history_data_key . ' row-' . ($bonus_history_data_key % 2 == 0 ? 'even' : 'odd') . '">';
      $output .= '<td>';
      $output .= $user_name;
      $output .= '</td>';
      $output .= '<td>';
      $output .= date('d.m.Y / H:i:s', $bonus_history_data_value[0]);
      $output .= '</td>';
      $output .= '<td>';
      $output .= $bonus_history_data_value[1];
      $output .= '</td>';
      $output .= '</tr>';
    }
    $output .= '</table>';
    $output .= '</div>';
  }
  else {
    $output .= '<div class="alert alert-block alert-danger messages error">Не удалось получить счёт пользователя</div>';
  }

  return $output;
}

function user_bonus_history_filter_form($form, &$form_state) {
  $form = [
    '#method' => 'get',
  ];

  $form['fields-wrapper'] = [
    '#type' => 'fieldset',
    '#title' => t('Фильтрация по имени пользователя'),
  ];

  $form['fields-wrapper']['filter-user-name'] = [
    '#type' => 'textfield',
    '#title' => t('Имя пользователя'),
    '#description' => '',
    '#default_value' => (isset($_GET['filter-user-name']) ? trim(
      $_GET['filter-user-name']
    ) : ''),
    '#attributes' => ['placeholder' => t('Укажите имя пользователя')],
  ];

  $form['fields-wrapper']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Применить'),
  ];

  return $form;
}

function user_bonus_history_filter_form_validate($form, &$form_state) {
  $user_name = trim($form_state['values']['filter-user-name']);
  if (preg_match('/^[-._0-9A-Za-z]+$/uis', $user_name) !== 1) {
    form_set_error(
      'filter-user-name',
      t(
        'Поле "Имя пользователя" заполнено некорректно. Разрешаются буквы латинского алфавита: точка, дефис, знак подчёркивания.'
      )
    );
  }
  elseif (user_load_by_name($user_name) === FALSE) {
    form_set_error(
      'filter-user-name',
      t(
        'Пользователь с данным именем не найден или не может быть загружен'
      )
    );
  }
}

function user_bonus_history_filter_form_submit($form, &$form_state) {
  //
}

function manager_adding_bonuses_form($form, &$form_state) {
  $form = [
    '#method' => 'post',
  ];

  $form['fields-wrapper'] = [
    '#type' => 'fieldset',
    '#title' => t('Зачисление бонусов на счёт пользователя'),
  ];

  $form['fields-wrapper']['inline-fields-wrapper']['user-name'] = [
    '#type' => 'textfield',
    '#title' => t('Имя пользователя'),
    '#description' => '',
    '#attributes' => ['placeholder' => t('Укажите имя пользователя')],
    '#required' => TRUE,
  ];

  $form['fields-wrapper']['inline-fields-wrapper']['bonus-amount'] = [
    '#type' => 'textfield',
    '#title' => t('Количество бонусов'),
    '#description' => '',
    '#attributes' => ['placeholder' => t('Укажите количество бонусов')],
    '#maxlength' => 5,
    '#required' => TRUE,
  ];

  $form['fields-wrapper']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Начислить'),
  ];

  return $form;
}

function manager_adding_bonuses_form_validate($form, &$form_state) {
  $user_name = trim($form_state['values']['user-name']);
  if (preg_match('/^[-._0-9A-Za-z]+$/uis', $user_name) !== 1) {
    form_set_error(
      'user-name',
      t(
        'Поле "Имя пользователя" заполнено некорректно. Разрешаются буквы латинского алфавита: точка, дефис, знак подчёркивания.'
      )
    );
  }
  elseif (user_load_by_name($user_name) === FALSE) {
    form_set_error(
      'user-name',
      t(
        'Пользователь с данным именем не найден или не может быть загружен'
      )
    );
  }

  $bonus_amount = trim($form_state['values']['bonus-amount']);
  if (preg_match(
      '/^[0-9]{1,5}$/uis',
      $bonus_amount
    ) !== 1 || $bonus_amount <= 0) {
    form_set_error(
      'bonus-amount',
      t(
        'Поле "Количество бонусов" заполнено некорректно, введите числовое значение больше нуля, до 5 знаков'
      )
    );
  }
}

function manager_adding_bonuses_form_submit($form, &$form_state) {
  $user_name = trim($form_state['values']['user-name']);
  $bonus_amount = trim($form_state['values']['bonus-amount']);

  $bonus_product = commerce_product_load_by_sku($user_name);

  if (!empty($bonus_product)) {
    $bonus_product->commerce_price['und'][0]['amount'] += $bonus_amount;
    $total_bonus_amount = $bonus_product->commerce_price['und'][0]['amount'];
    array_push(
      $bonus_product->field_bonus_history['und'],
      ['value' => (time() . '-' . $bonus_amount)]
    );
    commerce_product_save($bonus_product);

    $commands = [];
    $commands[] = ['command' => 'reloadPage'];
    $commands[] = [
      'message' => drupal_set_message(
        'Бонусы успешно зачислены. Количество бонусов на счёте пользователя "' . $user_name . '", после зачисления, составляет: ' . $total_bonus_amount,
        'status',
        FALSE
      ),
    ];
    return ['#type' => 'ajax', '#commands' => $commands];
  }
  else {
    $commands = [];
    $commands[] = ['command' => 'reloadPage'];
    $commands[] = [
      'message' => drupal_set_message(
        'Не удалось открыть (загрузить) бонусный счёт пользователя',
        'error',
        FALSE
      ),
    ];
    return ['#type' => 'ajax', '#commands' => $commands];
  }
}
