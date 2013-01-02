(function($){
	$('.submission').click(function(e) {
		if($('#url').val().split('.').pop() !== 'gif') {
			e.preventDefault();
			alert('Please sumbit a gif');
		}
	});

	$('#history_btn').click(function() {
		$.ajax({
			url:'admin.php',
			data:{
				history:1,
			},
		}).done(function( data ) {
			$('body').html(data);
			window.location.href = 'admin.php?history';
		});
	});
	
	$('.delete_this').click(function() {
		var row = $(this).parents('tr');
		var filename = row.find('img').attr('src').substring(9);
		$.ajax({
			url:'admin.php',
			data:{
				delete:1,
				file:filename,
			},
		}).done(function() {
			row.hide();
		});
	});

	$('.show_this').click(function() {
		var row = $(this).parents('tr');
		var filename = row.find('img').attr('src').substring(9);
		$.ajax({
			url:'admin.php',
			data:{
				display:1,
				file:filename,
			},
		}).done(function() {
			var date = new Date();
			row.find('.date').html(date.format('mmmm d, yyyy, h:MM TT'));
			row.addClass('success');
			setTimeout(function() {
				row.removeClass('success');
			},250);

		});
	});

	$('tbody tr').hoverIntent( {
		over: function() {
			var still = $(this).find('img'),
				mover;
			if($(this).hasClass('default_row')) {
				mover = '/default/default.gif';
			} else {
				mover = '/archive/' + still.attr('src').split('/').pop();
			}
			still.attr('src',mover);
		},
		out: function() {
			var mover = $(this).find('img'),
				still;
			if($(this).hasClass('default_row')) {
				still = '/default/default_frame.gif';
			} else {
				still = '/archive/frames/' + mover.attr('src').split('/').pop();
			}
			mover.attr('src',still);
		},
		sensitivity: 10,
	});

	$('#back').click(function() {
		window.location.href = 'admin.php';
	});

	$('#cron').click(function() {
		// ajax to run update db
		$.ajax({
			url:'admin.php',
			data:{
				update:1,
			},
		}).done(function( data ) {
			$('body').html(data);
		});
	});
	
	/*
	var stickyHeaderTop = $('#sticky').offset().top;
	$(window).scroll(function() {
		var wrapper = '<div class="sticky"><table><tbody></tbody></table></div>';
		if($(window).scrollTop() > stickyHeaderTop) {
			wrapper = $(wrapper).find('div').prepend($('#controls'));
			$(wrapper).find('tbody').append($('#sticky')).prependTo('body');
		} else {
			// unwrap
		}
	});
	*/
})(jQuery);