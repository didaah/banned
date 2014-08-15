<?php
//$Id$

// public banned_admin_list() {{{ 
/**
 * banned_admin_list
 *  屏蔽IP和帐号列表
 * 
 * @access public
 * @return void
 */
function banned_admin_list() {
  dd_set_tabs(array(
    l(t('banned', '屏蔽列表'), 'admin/banned'),
    l(t('banned', '新增屏蔽'), 'admin/banned/add'),
  ));

  $header = array(
    t('banned', '帐号/IP'),
    array('data' => t('banned', '屏蔽类型'), 'field' => 'b.type'),
    array('data' => t('banned', '屏蔽时间'), 'field' => 'b.created'),
    array('data' => t('banned', '到期时间'), 'field' => 'b.expired', 'sort' => 'desc'),
    array('data' => t('banned', '操作人'), 'field' => 'b.uid'),
    t('banned', '屏蔽原因'),
    t('banned', '取消原因'),
  );

  if ($fetch = pager_query('SELECT b.*, u.name FROM {banned_info} b INNER JOIN {users} u ON b.uid = u.uid' . table_sql($header), 50)) {
    $table = array();
    foreach ($fetch as $o) {
      $table[] = array(
        $o->account,
        $o->type == 1 ? t('banned', 'IP') : t('banned', '帐号'),
        format_date($o->created),
        format_date($o->expired),
        $o->name,
        $o->description,
        $o->free_description
      );
    }

    $output = theme('table', $header, $table, array('class' => 'center_table'));
    $output .= pager_view();
  }

  return $output;
}
// }}}

// public banned_admin_add() {{{ 
/**
 * banned_admin_add
 *  新增屏蔽
 * 
 * @access public
 * @return void
 */
function banned_admin_add() {
  dd_set_tabs(array(
    l(t('banned', '屏蔽列表'), 'admin/banned'),
    l(t('banned', '新增屏蔽'), 'admin/banned/add'),
  ));

  $form = array(
    'fields' => array(),
    'settings' => array(
      '#validate' => array('banned_admin_add_validate'),
      '#ajax_validate' => 1
    ),
  );

  $form['fields']['format'] = array(
    '#title' => t('banned', '屏蔽模式'),
    '#type' => 'radio',
    '#required' => 1,
    '#options' => array(
      t('banned', '普通'),
      t('banned', '通配符'),
      t('banned', '正则'),
    ),
    '#description' => t('banned', '普通：127.0.0.1，通配符：127.0.*.1，正则：/127.*?4/')
  );

  $form['fields']['type'] = array(
    '#title' => t('banned', '屏蔽类型'),
    '#type' => 'radio',
    '#required' => 1,
    '#options' => array(
      2 => t('banned', '帐号'),
      1 => t('banned', 'IP'),
    ),
    '#description' => t('banned', '帐号请输入 UID')
  );


  $form['fields']['account'] = array(
    '#title' => t('banned', '屏蔽内容'),
    '#type' => 'textfield',
    '#required' => 1,
    '#description' => t('banned', '待屏蔽的帐号或 IP'),
  );

  $form['fields']['description'] = array(
    '#title' => t('banned', '屏蔽原因'),
    '#type' => 'textfield',
    '#required' => 1,
  );

  $form['fields']['expired'] = array(
    '#title' => t('banned', '屏蔽时间'),
    '#type' => 'textfield',
    '#default_value' => 7,
    '#required' => 1,
  );

  $form['fields']['date'] = array(
    '#title' => t('banned', '时间单位'),
    '#type' => 'radio',
    '#required' => 1,
    '#default_value' => 'day',
    '#options' => array(
      'day' => t('banned', '天'),
      'week' => t('banned', '周'),
      'month' => t('banned', '月'),
      'year' => t('banned', '年'),
    ),
  );

  $form['fields']['button'] = array(
    '#type' => 'submit',
    '#submit' => 'banned_admin_add_submit',
  );

  return $form;
}
// }}}

// public banned_admin_add_validate(&$form, &$v) {{{ 
/**
 * banned_admin_add_validate
 *  验证屏蔽记录
 * 
 * @param mixed $form 
 * @param mixed $v 
 * @access public
 * @return void
 */
function banned_admin_add_validate(&$form, &$v) {
  switch ($v['_dida_values']->date) {
    case 'week':
      $days = $v['_dida_values']->expired*7;
    break;
    case 'month':
      $days = $v['_dida_values']->expired*31;
    break;
    case 'year':
      $days = $v['_dida_values']->expired*365;
    break;
    default:
      $days = $v['_dida_values']->expired;
    break;
  }
  
  $v['_dida_values']->expired = $days*86400+time();
  $v['_dida_values']->account = trim($v['_dida_values']->account);
}
// }}}

// public banned_admin_add_submit(&$form, &$v) {{{ 
/**
 * banned_admin_add_submit
 *  保存屏蔽记录
 * 
 * @param mixed $form 
 * @param mixed $v 
 * @access public
 * @return void
 */
function banned_admin_add_submit(&$form, &$v) {
  $v['_dida_values']->uid = $GLOBALS['user']->uid;
  $v['_dida_values']->created = time();
  if (db_write_record('banned_info', $v['_dida_values'])) {
    dd_set_message(t('message', '屏蔽成功'));
    cache_del('banned_list');
  } else {
    dd_set_message(t('message', '屏蔽失败'));
  }
}
// }}}

// public banned_admin_delete($id) {{{ 
/**
 * banned_admin_delete
 *  取消屏蔽
 * 
 * @param mixed $id 
 * @access public
 * @return void
 */
function banned_admin_delete($id) {
  // @TODO 待完善
  $form = array();
  return $form;
}
// }}}
