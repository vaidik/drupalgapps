<?php

class DrupalGappsOAuthRequestToken {
	private $consumerKey;
	private $consumerSecret;
	private $hd;
	private $scopes;
	private $xoauthDisplayname;
	private $callback;

	function __construct($consumerKey, $consumerSecret, $hd, $scopes, $xoauthDisplayname, $callback) {
		$this->consumerKey = $consumerKey;
		$this->consumerSecret = $consumerSecret;
		$this->hd = $hd;
		$this->scopes = $scopes;
		$this->xoauthDisplayname = $xoauthDisplayname;
		$this->callback = $callback;
	}

	function fetchRequestToken() {
		$consumer = new DrupalOAuthConsumer($this->consumerKey, $this->consumerSecret);
		$sigMethod = DrupalOAuthClient::signatureMethod('SHA1');
		$client = new DrupalOAuthClient($consumer, NULL, $sigMethod);

		$requestParams = array();
		$requestParams['scope'] = $this->scopes;
		$requestParams['xoauth_displayname'] = $this->xoauthDisplayname;

		$requestToken = $client->getRequestToken('https://www.google.com/accounts/OAuthGetRequestToken', array(
			'callback' => $this->callback,
			'params' => $requestParams,
		));

		$this->token = new stdClass;
		$this->token->key = $requestToken->key;
		$this->token->secret = $requestToken->secret;

		$params = array();
		$params['hd'] = $this->hd;

		$this->authURL = $client->getAuthorizationUrl('https://www.google.com/accounts/OAuthAuthorizeToken', array(
			'params' => $authURL,
		));
	}

	function redirectToAuthURL() {
		drupal_goto($this->authURL);
	}
}

class DrupalGappsOAuthAccessToken {
	private $consumerKey;
	private $consumerSecret;
	private $token;
	private $oauthVerifier;
	private $oauthToken;

	function __construct($consumerKey, $consumerSecret, $token, $oauthVerifier, $oauthToken) {
		$this->consumerKey = $consumerKey;
		$this->consumerSecret = $consumerSecret;
		$this->token = $token;
		$this->oauthVerifier = $oauthVerifier;
		$this->oauthToken = $oauthToken;
	}

	function fetchAccessToken() {
		$consumer = new DrupalOAuthConsumer($this->consumerKey, $this->consumerSecret);

		$requestToken = new OAuthToken($this->token->key, $this->token->secret);

		$client = new DrupalOAuthClient($consumer, $requestToken);
		$sigMethod = $client->signatureMethod('sha1');

		$params = array();
	  $params['oauth_verifier'] = $this->oauthVerifier;
  	$params['oauth_token'] = $this->oauthToken;

	  $access = $client->getAccessToken('https://www.google.com/accounts/OAuthGetAccessToken', array('params' => $params, 'verifier' => $this->oauthVerifier));

		$this->accessToken = new stdClass;
		$this->accessToken->key = $access->key;
		$this->accessToken->secret = $access->secret;
	}
}
