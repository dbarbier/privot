# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

inherit eutils

DESCRIPTION="rot is a R package that allow OpenTURNS to compute with R"
HOMEPAGE="http://openturns.org"
SRC_URI="mirror://sourceforge/openturns/openturns-0.10.0.tar.gz"

LICENSE="LGPL"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE=""

DEPEND="dev-lang/R"

RDEPEND="${DEPEND}"

src_unpack() {
	unpack ${A}
}

src_compile() {
	export R_HOME="/usr/$(get_libdir)/R"
}

src_install() {
	export R_LIBS="${R_HOME}/library"
	keepdir ${R_LIBS_SITE}

	cd openturns-0.10.0/utils
	R CMD INSTALL -l "${D}${R_LIBS}" rotRPackage_1.4.3.tar.gz || die "install failed"
}

