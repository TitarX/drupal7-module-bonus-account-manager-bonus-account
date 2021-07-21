<?php

/**
* Page callback
*/
function manager_customers_accounts_page()
{
	drupal_add_js(drupal_get_path('module', 'manager_bonus_account').'/js/manager_customers_accounts_page.js');
	drupal_add_css(drupal_get_path('module', 'manager_bonus_account').'/css/manager_customers_accounts_page.css');
	
	$output = '';
	
	$user_bonus_account_filter_form = drupal_get_form('user_bonus_account_filter_form');
	$output .= drupal_render($user_bonus_account_filter_form);
	
	$user_bonus_account_filter_name = 'all';
	if(isset($_GET['user-bonus-account-filter-name']) && !empty($_GET['user-bonus-account-filter-name']))
	{
		$user_bonus_account_filter_name = trim($_GET['user-bonus-account-filter-name']);
	}
	
	$user_bonus_account_filter_email = 'all';
	if(isset($_GET['user-bonus-account-filter-email']) && !empty($_GET['user-bonus-account-filter-email']))
	{
		$user_bonus_account_filter_email = trim($_GET['user-bonus-account-filter-email']);
	}
	
	$user_bonus_account_filter_tel = 'all';
	if(isset($_GET['user-bonus-account-filter-tel']) && !empty($_GET['user-bonus-account-filter-tel']))
	{
		$user_bonus_account_filter_tel = trim($_GET['user-bonus-account-filter-tel']);
	}
	
	$output .= '<div id="customers_wrapper">';
	$output .= views_embed_view('view_bonus_account_users', 'block', $user_bonus_account_filter_name, $user_bonus_account_filter_email, $user_bonus_account_filter_tel);
	$output .= '</div>';
	
	return $output;
}

function user_bonus_account_filter_form($form, &$form_state)
{
	$form = array(
		'#method' => 'get'
	);
	
	$form['fields-wrapper'] = array(
		'#type' => 'fieldset',
		'#title' => t('Фильтрация пользователей')
	);
	
	$form['fields-wrapper']['user-bonus-account-filter-name'] = array(
		'#type' => 'textfield',
		'#title' => t('Имя пользователя'),
		'#description' => '',
		'#default_value' => (isset($_GET['user-bonus-account-filter-name']) ? trim($_GET['user-bonus-account-filter-name']) : ''),
		'#attributes' => array('placeholder' => t('Укажите имя пользователя'))
	);
	
	$form['fields-wrapper']['user-bonus-account-filter-email'] = array(
		'#type' => 'textfield',
		'#title' => t('Email пользователя'),
		'#description' => '',
		'#default_value' => (isset($_GET['user-bonus-account-filter-email']) ? trim($_GET['user-bonus-account-filter-email']) : ''),
		'#attributes' => array('placeholder' => t('Укажите email пользователя'))
	);
	
	$form['fields-wrapper']['user-bonus-account-filter-tel'] = array(
		'#type' => 'textfield',
		'#title' => t('Телефон пользователя'),
		'#description' => '',
		'#default_value' => (isset($_GET['user-bonus-account-filter-tel']) ? trim($_GET['user-bonus-account-filter-tel']) : ''),
		'#attributes' => array('placeholder' => t('Укажите телефон пользователя'))
	);
	
	$form['fields-wrapper']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Применить')
	);
	
	return $form;
}

function user_bonus_account_filter_form_validate($form, &$form_state)
{
	$user_bonus_account_filter_name = trim($form_state['values']['user-bonus-account-filter-name']);
	if(preg_match('/[\'"<?>]/uis', $user_bonus_account_filter_name) === 1)
	{
		form_set_error('user-bonus-account-filter-name', t('Поле "Имя пользователя" содержит недопустимые символы'));
	}
	
	$user_bonus_account_filter_email = trim($form_state['values']['user-bonus-account-filter-email']);
	if(preg_match('/[\'"<?>]/uis', $user_bonus_account_filter_email) === 1)
	{
		form_set_error('user-bonus-account-filter-email', t('Поле "Email пользователя" содержит недопустимые символы'));
	}
	
	$user_bonus_account_filter_tel = trim($form_state['values']['user-bonus-account-filter-tel']);
	if(preg_match('/[\'"<?>]/uis', $user_bonus_account_filter_tel) === 1)
	{
		form_set_error('user-bonus-account-filter-tel', t('Поле "Телефон пользователя" содержит недопустимые символы'));
	}
}

function user_bonus_account_filter_form_submit($form, &$form_state)
{
	//
}
