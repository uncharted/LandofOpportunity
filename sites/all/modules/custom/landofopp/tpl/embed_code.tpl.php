<?php 
/*
* embed code template
* 
*/
?>
<?php if (!empty($node)) : ?>
<?php 
	$output = '';
	global $base_url;
	global $base_path;
	$data_url = url('node/'.$node->nid);
	$pos = strpos($data_url,$base_path);
	if ($pos !== false) {
		$data_url = substr_replace($data_url,'',$pos,strlen($base_path));
	}
	
	$site_url = url('<front>',array('absolute'=>TRUE));
	$node_url = url('<front>',array('absolute'=>TRUE,'fragment'=>'/'.$data_url));

	$image = landofopp_get_original_image($node);
	if (!empty($image)) {
		$preview = theme_image_style(array('style_name' => 'embed', 'path' => $image['uri'],'width'=>$image['metadata']['width'],'height'=>$image['metadata']['height'],'attributes'=>array('style'=>'width:100%;')));
	
		$theme_path = $base_url.'/'.drupal_get_path('theme','landofopportunity');
	
		$target = 'target="_blank"';
		$styles_play = 'width:50px; height:50px; text-indent:-99999px; overflow:hiddden; background:url('.$theme_path.'/images/bg-arrow-small.png); display:block; position:absolute; top:50%; left:50%; margin:-25px 0 0 -25px';
		$style_logo  = 'background: url('.$theme_path.'/images/logo.png); text-indent:-99999px; height: 45px; left: 35px; position: absolute; top: 20px; width: 200px;';

		$output = '<div style="position:relative; width:'.$width.'px; height:'.$height.'px">';
	
		$output .= $preview;
		$output .= '<a href="'.$node_url.'" '.$target.' style="'.$styles_play.'">play</a>';
		$output .= '<a href="'.$site_url.'" '.$target.' style="'.$style_logo.'">logo</a>';
		$output .= '</div>';
		
	}
	
	echo $output;
?>
<?php endif; ?>