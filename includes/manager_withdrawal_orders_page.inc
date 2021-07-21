<?php

/**
 * Page callback
 */
function manager_withdrawal_orders_page() {
  drupal_add_js(
    drupal_get_path(
      'module',
      'manager_bonus_account'
    ) . '/js/manager_withdrawal_orders_page.js'
  );
  drupal_add_css(
    drupal_get_path(
      'module',
      'manager_bonus_account'
    ) . '/css/manager_withdrawal_orders_page.css'
  );

  $output = '';

  $bonus_withdrawal_orders_form = drupal_get_form(
    'bonus_withdrawal_orders_form'
  );
  $output .= drupal_render($bonus_withdrawal_orders_form);

  $bonus_withdrawal_orders_user_name = 'all';
  if (isset($_GET['bonus-withdrawal-orders-user-name']) && !empty($_GET['bonus-withdrawal-orders-user-name'])) {
    $bonus_withdrawal_orders_user_name = trim(
      $_GET['bonus-withdrawal-orders-user-name']
    );
  }

  $bonus_withdrawal_orders_number = 'all';
  if (isset($_GET['bonus-withdrawal-orders-number']) && !empty($_GET['bonus-withdrawal-orders-number'])) {
    $bonus_withdrawal_orders_number = trim(
      $_GET['bonus-withdrawal-orders-number']
    );
  }

  $bonus_withdrawal_orders_status = 'all';
  if (isset($_GET['bonus-withdrawal-orders-status']) && !empty($_GET['bonus-withdrawal-orders-status'])) {
    $bonus_withdrawal_orders_status = trim(
      $_GET['bonus-withdrawal-orders-status']
    );
  }

  // Смена статуса заявки -
  if (isset($_GET['bonus-order-zid']) && !empty($_GET['bonus-order-zid']) && !isset($_GET['bonus-order-oid'])) {
    $bonus_order_zid = trim($_GET['bonus-order-zid']);

    $bonus_commerce_order = commerce_order_load($bonus_order_zid);
    if ($bonus_commerce_order !== FALSE) {
      $bonus_order_user_name = $bonus_commerce_order->field_bonus_order_user_name['und'][0]['value'];
      $bonus_commerce_product = commerce_product_load_by_sku(
        $bonus_order_user_name
      );
      if ($bonus_commerce_product !== FALSE) {
        $bonus_commerce_product->commerce_price['und'][0]['amount'] -= $bonus_commerce_order->field_amount_bonus_for_order['und'][0]['value'];
        $total_bonuses_amount = $bonus_commerce_product->commerce_price['und'][0]['amount'];
        commerce_product_save($bonus_commerce_product);
        commerce_order_status_update($bonus_commerce_order, 'completed');
        drupal_set_message(
          "Бонусы успешно списаны сосчёта пользователя $bonus_order_user_name. Количество бонусов, на счёте пользователя $bonus_order_user_name, после списания составляет: $total_bonuses_amount. Статус заявки № $bonus_order_zid успешно изменён на \"Завершено\"",
          'status',
          FALSE
        );
      }
      else {
        drupal_set_message(
          "Не удалось изменить статус заявки № $bonus_order_zid на \"Завершено\"",
          'error',
          FALSE
        );
      }
    }
    else {
      drupal_set_message(
        "Не удалось изменить статус заявки № $bonus_order_zid на \"Завершено\"",
        'error',
        FALSE
      );
    }

    $bonus_withdrawal_orders_number = $bonus_order_zid;
  }
  elseif (isset($_GET['bonus-order-oid']) && !empty($_GET['bonus-order-oid']) && !isset($_GET['bonus-order-zid'])) {
    $bonus_order_oid = trim($_GET['bonus-order-oid']);

    $bonus_commerce_order = commerce_order_load($bonus_order_oid);
    if ($bonus_commerce_order !== FALSE) {
      commerce_order_status_update($bonus_commerce_order, 'canceled');
      drupal_set_message(
        "Статус заявки № $bonus_order_oid успешно изменён на \"Отменено\"",
        'status',
        FALSE
      );
    }
    else {
      drupal_set_message(
        "Не удалось изменить статус заявки № $bonus_order_oid на \"Отменено\"",
        'error',
        FALSE
      );
    }

    $bonus_withdrawal_orders_number = $bonus_order_oid;
  }
  // - Смена статуса заявки

  $output .= '<div id="bonus_withdrawal_history_table_wrapper">';
  $output .= views_embed_view(
    'view_withdrawal_bonuses_orders',
    'block_1',
    $bonus_withdrawal_orders_user_name,
    $bonus_withdrawal_orders_number,
    $bonus_withdrawal_orders_status
  );
  $output .= '</div>';

  return $output;
}

function bonus_withdrawal_orders_form($form, &$form_state) {
  $form = [
    '#method' => 'get',
  ];

  $form['fields-wrapper'] = [
    '#type' => 'fieldset',
    '#title' => t('Фильтрация заявок'),
  ];

  $form['fields-wrapper']['bonus-withdrawal-orders-user-name'] = [
    '#type' => 'textfield',
    '#title' => t('Имя пользователя'),
    '#description' => '',
    '#default_value' => (isset($_GET['bonus-withdrawal-orders-user-name']) ? trim(
      $_GET['bonus-withdrawal-orders-user-name']
    ) : ''),
    '#attributes' => ['placeholder' => t('Укажите имя пользователя')],
  ];

  $form['fields-wrapper']['bonus-withdrawal-orders-number'] = [
    '#type' => 'textfield',
    '#title' => t('Номер заказа'),
    '#description' => '',
    '#default_value' => (isset($_GET['bonus-withdrawal-orders-number']) ? trim(
      $_GET['bonus-withdrawal-orders-number']
    ) : ''),
    '#attributes' => ['placeholder' => t('Укажите номер заказа')],
  ];

  $form['fields-wrapper']['bonus-withdrawal-orders-status'] = [
    '#type' => 'select',
    '#title' => t('Статус заказа'),
    '#description' => '',
    '#options' => [
      'all' => t('Любой'),
      'pending' => t('В ожидании'),
      //'processing' => t('Обработка'),
      'canceled' => t('Отменено'),
      'completed' => t('Завершено'),
    ],
    '#default_value' => (isset($_GET['bonus-withdrawal-orders-status']) ? trim(
      $_GET['bonus-withdrawal-orders-status']
    ) : ''),
  ];

  $form['fields-wrapper']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Применить'),
  ];

  return $form;
}

function bonus_withdrawal_orders_form_validate($form, &$form_state) {
  $bonus_withdrawal_orders_user_name = trim(
    $form_state['values']['bonus-withdrawal-orders-user-name']
  );
  if (preg_match('/[\'"<?>]/uis', $bonus_withdrawal_orders_user_name) === 1) {
    form_set_error(
      'bonus-withdrawal-orders-user-name',
      t('Поле "Имя пользователя" содержит недопустимые символы')
    );
  }

  $bonus_withdrawal_orders_number = trim(
    $form_state['values']['bonus-withdrawal-orders-number']
  );
  if (preg_match('/^[0-9]*$/uis', $bonus_withdrawal_orders_number) !== 1) {
    form_set_error(
      'bonus-withdrawal-orders-number',
      t(
        'Поле "Номер заказа" содержит недопустимые символы, разрешаются только цифры'
      )
    );
  }
}

function bonus_withdrawal_orders_form_submit($form, &$form_state) {
  //
}
