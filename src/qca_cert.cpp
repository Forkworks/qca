/*
 * qca_cert.cpp - Qt Cryptographic Architecture
 * Copyright (C) 2004  Justin Karneges
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "qca.h"

#include <qdatetime.h>
#include <qregexp.h>
#include "qcaprovider.h"

namespace QCA {

Provider::Context *getContext(const QString &type, const QString &provider);

//----------------------------------------------------------------------------
// Certificate
//----------------------------------------------------------------------------
// (adapted from kdelibs) -- Justin
static bool cnMatchesAddress(const QString &_cn, const QString &peerHost)
{
	QString cn = _cn.stripWhiteSpace().lower();
	QRegExp rx;

	// Check for invalid characters
	if(QRegExp("[^a-zA-Z0-9\\.\\*\\-]").search(cn) >= 0)
		return false;

	// Domains can legally end with '.'s.  We don't need them though.
	while(cn.endsWith("."))
		cn.truncate(cn.length()-1);

	// Do not let empty CN's get by!!
	if(cn.isEmpty())
		return false;

	// Check for IPv4 address
	rx.setPattern("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");
	if(rx.exactMatch(peerHost))
		return peerHost == cn;

	// Check for IPv6 address here...
	rx.setPattern("^\\[.*\\]$");
	if(rx.exactMatch(peerHost))
		return peerHost == cn;

	if(cn.contains('*')) {
		// First make sure that there are at least two valid parts
		// after the wildcard (*).
		QStringList parts = QStringList::split('.', cn, false);

		while(parts.count() > 2)
			parts.remove(parts.begin());

		if(parts.count() != 2) {
			return false;  // we don't allow *.root - that's bad
		}

		if(parts[0].contains('*') || parts[1].contains('*')) {
			return false;
		}

		// RFC2818 says that *.example.com should match against
		// foo.example.com but not bar.foo.example.com
		// (ie. they must have the same number of parts)
		if(QRegExp(cn, false, true).exactMatch(peerHost) &&
			QStringList::split('.', cn, false).count() ==
			QStringList::split('.', peerHost, false).count())
			return true;

		return false;
	}

	// We must have an exact match in this case (insensitive though)
	// (note we already did .lower())
	if(cn == peerHost)
		return true;
	return false;
}

Certificate::Certificate()
{
}

bool Certificate::isNull() const
{
	return (!context() ? true : false);
}

int Certificate::version() const
{
	return ((CertContext *)context())->version();
}

QDateTime Certificate::notValidBefore() const
{
	return ((CertContext *)context())->notValidBefore();
}

QDateTime Certificate::notValidAfter() const
{
	return ((CertContext *)context())->notValidAfter();
}

Certificate::Info Certificate::subjectInfo() const
{
	return ((CertContext *)context())->subjectInfo();
}

Certificate::Info Certificate::issuerInfo() const
{
	return ((CertContext *)context())->issuerInfo();
}

QString Certificate::commonName() const
{
	return subjectInfo()["CN"];
}

QBigInteger Certificate::serialNumber() const
{
	return ((CertContext *)context())->serialNumber();
}

PublicKey Certificate::subjectPublicKey() const
{
	PKeyContext *c = ((CertContext *)context())->subjectPublicKey();
	PublicKey key;
	key.change(c);
	return key;
}

QSecureArray Certificate::toDER() const
{
	return ((CertContext *)context())->toDER();
}

QString Certificate::toPEM() const
{
	return ((CertContext *)context())->toPEM();
}

Certificate Certificate::fromDER(const QSecureArray &a, const QString &provider)
{
	Certificate c;
	CertContext *cc = (CertContext *)getContext("cert", provider);
	if(cc->fromDER(a) == CertContext::Good)
		c.change(cc);
	return c;
}

Certificate Certificate::fromPEM(const QString &s, const QString &provider)
{
	Certificate c;
	CertContext *cc = (CertContext *)getContext("cert", provider);
	if(cc->fromPEM(s) == CertContext::Good)
		c.change(cc);
	return c;
}

bool Certificate::matchesAddress(const QString &realHost) const
{
	QString peerHost = realHost.stripWhiteSpace();
	while(peerHost.endsWith("."))
		peerHost.truncate(peerHost.length()-1);
	peerHost = peerHost.lower();

	if(cnMatchesAddress(commonName(), peerHost))
		return true;
	return false;
}

//----------------------------------------------------------------------------
// CRL
//----------------------------------------------------------------------------
CRL::CRL()
{
}

bool CRL::isNull() const
{
	return (!context() ? true : false);
}

QSecureArray CRL::toDER() const
{
	return ((CRLContext *)context())->toDER();
}

QString CRL::toPEM() const
{
	return ((CRLContext *)context())->toPEM();
}

CRL CRL::fromDER(const QSecureArray &a, const QString &provider)
{
	CRL c;
	CRLContext *cc = (CRLContext *)getContext("crl", provider);
	if(cc->fromDER(a) == CRLContext::Good)
		c.change(cc);
	return c;
}

CRL CRL::fromPEM(const QString &s, const QString &provider)
{
	CRL c;
	CRLContext *cc = (CRLContext *)getContext("crl", provider);
	if(cc->fromPEM(s) == CRLContext::Good)
		c.change(cc);
	return c;
}

//----------------------------------------------------------------------------
// Store
//----------------------------------------------------------------------------
Store::Store(const QString &provider)
:Algorithm("store", provider)
{
}

void Store::addCertificate(const Certificate &cert, bool trusted)
{
	((StoreContext *)context())->addCertificate(*((CertContext *)cert.context()), trusted);
}

void Store::addCRL(const CRL &crl)
{
	((StoreContext *)context())->addCRL(*((CRLContext *)crl.context()));
}

CertValidity Store::validate(const Certificate &cert, CertUsage u) const
{
	return ((StoreContext *)context())->validate(*((CertContext *)cert.context()), u);
}

}