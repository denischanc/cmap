#!/bin/bash

OPTIONS=()

NO_TOKEN_ERR_NB=99
NO_ID_ERR_NB=98
USAGE_ERR_NB=97

GITHUB_OWNER=denischanc
GITHUB_REPO=cmap

################################################################################
################################################################################

usage() {
	cat << _EOF_
usage: $0 [$(echo ${OPTIONS[@]} | tr ' ' '|')]
_EOF_
}

################################################################################
################################################################################

OPTIONS+=(api_call)

api_call_usage() {
	cat << _EOF_
usage: $0 api_call [host:api] [method:GET] [resource] ([curl arg]...)
_EOF_
}

api_call_ARGS_MIN_NB=3

api_call() {
	local HOST=${1:-api}
	local METHOD=${2:-GET}
	local RESOURCE=$3
	shift 3
	local CURL_ARGS=()
	CURL_ARGS+=(-L --no-progress-meter)
	CURL_ARGS+=(-X $METHOD)
	CURL_ARGS+=(-H "Authorization: Bearer $GITHUB_AUTH_TOKEN")
	local GITHUB_PATH=/repos/$GITHUB_OWNER/$GITHUB_REPO$RESOURCE
	CURL_ARGS+=(https://$HOST.github.com$GITHUB_PATH)
	while [ $# -ne 0 ]
	do
		CURL_ARGS+=("$1")
		shift
	done
	curl "${CURL_ARGS[@]}"
}

################################################################################
################################################################################

OPTIONS+=(list_releases)

list_releases_usage() {
	cat << _EOF_
usage: $0 list_releases
_EOF_
}

list_releases_ARGS_MIN_NB=0

list_releases() {
	api_call "" "" /releases
}

################################################################################
################################################################################

OPTIONS+=(create_release)

create_release_usage() {
	cat << _EOF_
usage: $0 create_release [tag name] [release name]
_EOF_
}

create_release_ARGS_MIN_NB=2

create_release() {
	local TAG_NAME=$1
	local RELEASE_NAME=$2
	local DATA="{\"tag_name\":\"$TAG_NAME\",\"name\":\"$RELEASE_NAME\"}"
	api_call "" POST /releases -d "$DATA"
}

################################################################################
################################################################################

OPTIONS+=(release_id)

release_id_usage() {
	cat << _EOF_
usage: $0 release_id [tag name]
_EOF_
}

release_id_ARGS_MIN_NB=1

release_id() {
	local TAG_NAME=$1
	local ID=$(api_call "" "" /releases/tags/$TAG_NAME 2> /dev/null | \
		jq -r .id)
	if [ -n "$ID" -a "$ID" != "null" ]
	then
		echo $ID
	else
		return $NO_ID_ERR_NB
	fi
}

################################################################################
################################################################################

OPTIONS+=(upload_release_asset)

upload_release_asset_usage() {
	cat << _EOF_
usage: $0 upload_release_asset [release id] [asset name] [file] [type]
_EOF_
}

upload_release_asset_ARGS_MIN_NB=4

upload_release_asset() {
	local RELEASE_ID=$1
	local ASSET_NAME=$2
	local FILE=$3
	local TYPE=$4
	api_call uploads POST "/releases/$RELEASE_ID/assets?name=$ASSET_NAME" \
		-H "Content-Type: $TYPE" --data-binary "@$FILE"
}

################################################################################
################################################################################

OPTIONS+=(delete_release)

delete_release_usage() {
	cat << _EOF_
usage: $0 delete_release [release id]
_EOF_
}

delete_release_ARGS_MIN_NB=1

delete_release() {
	local RELEASE_ID=$1
	api_call "" DELETE /releases/$RELEASE_ID
}

################################################################################
################################################################################

OPTIONS+=(run_wkf)

run_wkf_usage() {
	cat << _EOF_
usage: $0 run_wkf [workflow name] [branch]
_EOF_
}

run_wkf_ARGS_MIN_NB=2

run_wkf() {
	local NAME=$1
	local BRANCH=$2
	local DATA="{\"ref\":\"$BRANCH\"}"
	api_call "" POST /actions/workflows/$NAME/dispatches -d "$DATA"
}

################################################################################
################################################################################

if [ -z "$GITHUB_AUTH_TOKEN" ]
then
	echo "No token in GITHUB_AUTH_TOKEN env var !!!"
	exit $NO_TOKEN_ERR_NB
fi

for o in ${OPTIONS[@]}
do
	if [ "$1" = "$o" ]
	then
		shift
		eval ARGS_MIN_NB=\$${o}_ARGS_MIN_NB
		if [ "$1" = "--help" -o $# -lt $ARGS_MIN_NB ]
		then
			${o}_usage
			exit $USAGE_ERR_NB
		else
			$o "$@"
			exit $?
		fi
	fi
done

usage
exit $USAGE_ERR_NB
