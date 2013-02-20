# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

inherit eutils

DESCRIPTION="Open source initiative to Treat Uncertainties, Risks'N Statistics in a structured industrial approach."
HOMEPAGE="http://openturns.org"

SRC_URI="mirror://sourceforge/${PN}/${P}.tar.gz"

IUSE="python gui doc validation"
LICENSE="LGPL"
SLOT="0"
KEYWORDS="~x86 ~amd64"

COMMON_DEPEND="sys-devel/gcc
	sys-devel/automake
	dev-lang/R
	sci-libs/lapack-atlas
	<x11-libs/qt-4
	sci-mathematics/rot
	python? ( >=dev-lang/python-2.3 )
	"

DEPEND="${COMMON_DEPEND}
	dev-libs/xerces-c
	dev-libs/boost
	"
RDEPEND="${COMMON_DEPEND}"

CXXFLAGS="$CXXFLAGS -mno-sse2"

src_compile() {
	econf -C \
		$(use_enable python python) \
		$(use_enable gui gui) \
		$(use_enable doc doc) \
		$(use_enable validation validation) \
		|| die "econf failed"
	emake || die "emake failed"
}

src_install() {
	emake DESTDIR="${D}" install || die "install failed"

#	dodoc FAQ NEWS README
#	dohtml EXTENDING.html ctags.html
}

#pkg_setup() {
#	elog "Cream comes with several dictionaries for spell checking. In"
#	elog "all cases, at least a small English dictionary will be installed."
#	elog
#	elog "To specify which optional dictionaries are installed, set the"
#	elog "LINGUAS variable in /etc/make.conf. For example, to install full"
#	elog "English and French dictionaries, use:"
#	elog "    LINGUAS=\"en fr\""
#	elog
#	elog "Available dictionaries are:"
#	for dict in "English en" "French fr" "German de" "Spanish es" ; do
#		# portage bug: shouldn't get a QA notice for linguas stuff...
#		elog "    ${dict% *} \t(${dict#* }) $( ( \
#			use linguas_${dict#* } &>/dev/null && \
#			echo '(Will be installed)' ) || echo '(Will not be installed)' )"
#	done
#	elog
#	# give the user time to cancel if necessary
#	epause
#}
