package  
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.text.TextField;
	import flash.external.ExternalInterface;
	import flash.net.LocalConnection;
	
	public class GameTimer extends Sprite
	{
		private var _time:TextField;
		private var _testTime:int;
		
		public function GameTimer() 
		{
			_time = time;
			ExternalInterface.addCallback('setTime', setTime);
		}
		
		public function setTime(seconds:Number):void
		{
			_time.text = '' + int(seconds) ;
		}
	}
	
}